#include "handler.hpp"

handler::handler(tcp_client _client, ip_addr server_ip)
    : client(std::move(_client)),
      server_ip(server_ip),
      data_connection_listener(server_ip, 0),
      data_client(nullptr) {
  this->port = this->data_connection_listener.get_port();
}

void handler::handle() {
  std::vector<uint8_t> readed_data;
  std::vector<uint8_t> buf(1024);

  while (true) {
    while (true) {
      ssize_t read_bytes_count = this->client.read_chunk(buf);

      if (read_bytes_count > 0) {
        readed_data.insert(readed_data.end(), buf.begin(),
                           buf.begin() + read_bytes_count);
      } else {
        break;
      }
    }

    if (readed_data.size() <= 0) {
      continue;
    }

    if (this->task.has_value() && this->task.value().is_error_happend()) {
      this->client.send_error(response_type::SEND_DATA_FAILED);
      this->client.send_error_message(this->task.value().get_error_message());
      readed_data.clear();
      continue;
    }
    this->handle_request(readed_data);

    readed_data.clear();
  }
}

void handler::handle_request(std::vector<uint8_t> readed_data) {
  request_type request = request_type_from_byte(readed_data[0]);

  if (DATA_RECONNECT == request) {
    this->generate_port();
    this->client.send_port(this->port);
    this->data_client = boost::shared_ptr<tcp_client>(
        new tcp_client(this->data_connection_listener.accept_client()));
    this->task.reset();
    return;
  }

  if (PING == request) {
    this->client.send_ping();
    return;
  }

  if (nullptr == this->data_client) {
    this->client.send_error(response_type::CHANNEL_IS_BUSY);
    return;
  }

  if (GET_DATA == request) {
    if (this->data_client->is_closed()) {
      this->client.send_error(response_type::CHANNEL_WASNT_CREATED);
      return;
    }

    if (!this->task.has_value()) {
      std::string path(reinterpret_cast<char *>(readed_data.data()),
                       readed_data.size());

      this->task.emplace(boost::weak_ptr<tcp_client>(this->data_client), path);
      this->task->run();

    } else {
      this->client.send_error(response_type::CHANNEL_IS_BUSY);
    }
    // this->handle_get_data_request(this->data_sender.value(), readed_data);
    return;
  }

  this->client.send_error(response_type::INVALID_REQUEST);
}

void handler::generate_port() {
  this->data_connection_listener = tcp_listener(this->server_ip, 0);
  this->port = this->data_connection_listener.get_port();
}
