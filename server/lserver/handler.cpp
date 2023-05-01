#include "handler.hpp"
#include "message_type.hpp"
#include <boost/algorithm/string.hpp>

handler::handler(tcp_client _client, ip_addr server_ip)
    : client(std::move(_client)), server_ip(server_ip),
      data_connection_listener(server_ip, 0), data_client(nullptr) {
  this->port = this->data_connection_listener.get_port();
}

void handler::handle() {
  std::vector<uint8_t> readed_data;
  std::vector<uint8_t> buf(1024);

  while (true) {
    readed_data.clear();

    while (true) {
      ssize_t read_bytes_count = this->client.read_chunk(buf);

      if (read_bytes_count == 0) {
        break;
      }

      readed_data.insert(readed_data.end(), buf.begin(),
                         buf.begin() + read_bytes_count);
    }

    if (this->task.has_value() && this->task.value().is_error_happend()) {
      this->client.send_error_with_message(
          response_type::SEND_DATA_FAILED,
          this->task.value().get_error_message());
      this->task.reset();

      continue;
    }

    if (readed_data.size() == 0) {
      continue;
    }

    this->handle_request(readed_data);
  }
}

void handler::handle_request(std::vector<uint8_t> readed_data) {
  request_type request = request_type_from_byte(readed_data[0]);

  if (request == REQUEST_TYPE::DATA_RECONNECT) {
    this->handle_data_channel_reconnect();
    return;
  }

  if (request == REQUEST_TYPE::PING) {
    this->client.send_pong();
    return;
  }

  if (request == REQUEST_TYPE::GET_DATA) {
    this->handle_get_data(readed_data);
    return;
  }

  this->client.send_error(response_type::INVALID_REQUEST);
}

void handler::handle_data_channel_reconnect() {
  this->generate_port();
  this->client.send_port(this->port);
  this->data_client = boost::shared_ptr<tcp_client>(
      new tcp_client(this->data_connection_listener.accept_client()));
  this->task.reset();
}

void handler::generate_port() {
  this->data_connection_listener = tcp_listener(this->server_ip, 0);
  this->port = this->data_connection_listener.get_port();
}

void handler::handle_get_data(std::vector<uint8_t> readed_data) {
  if (this->task.has_value() && this->task->is_finished()) {
    this->task.reset();
  }

  if (this->data_client == nullptr) {
    this->client.send_error(response_type::NO_DATA_CHANNEL);
    return;
  }

  if (this->task.has_value()) {
    this->client.send_error(response_type::DATA_CHANNEL_IS_BUSY);
    return;
  }

  std::string path(reinterpret_cast<char *>(readed_data.data()),
                   readed_data.size());
  boost::trim_right(path);

  if (path.size() == 1) {
    this->client.send_error_with_message(response_type::SEND_DATA_FAILED,
                                         "required fs entry path");
    return;
  }

  path.erase(path.begin(), path.begin() + 1);

  this->task.emplace(boost::weak_ptr<tcp_client>(this->data_client), path);
  this->task->run();
}
