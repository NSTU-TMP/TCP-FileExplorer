#include "connection.hpp"
#include "fs_task.hpp"
#include "info_client.hpp"
#include "messages_types.hpp"
#include "tcp_client.hpp"
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <exception>

connection::connection(boost::shared_ptr<class logger> _logger,
                       ip_addr _server_ip_addr, tcp_client _tcp_client)
    : lg(_logger), client{_logger, std::move(_tcp_client)},
      server_ip_addr(_server_ip_addr) {}

void connection::listen() {
  try {
    while (true) {
      if (auto request = this->client.read()) {
        this->handle_request(request->first, request->second);
      }

      this->check_task_for_error();
    }
  } catch (std::exception &ex) {
    this->lg->error("connection listener", ex.what());
  }
}

void connection::check_task_for_error() {
  if (!this->current_task.has_value()) {
    return;
  }

  auto error = this->current_task.value()->get_error();
  if (!error) {
    return;
  }

  this->current_task.reset();
  this->client.send_error(std::string(error.value()));
}

void connection::handle_request(info_request_type request_type,
                                std::vector<uint8_t> bytes) {
  switch (request_type) {
  case info_request_type::PING:
    this->lg->info("connection listener", "handle PING info request");
    this->client.send_pong(std::move(bytes));
    break;
  case info_request_type::GET_FS_ENTRY:
    this->lg->info("connection listener", "handle GET_FS_ENTRY info request");
    this->handle_get_fs_entry(std::move(bytes));
    break;
  case info_request_type::RECONNECT_DATA_CHANNEL:
    this->lg->info("connection listener",
                   "handle RECONNECT_DATA_CHANNEL info request");
    this->handle_reconnect_data_channel();
    break;
  }
}

void connection::handle_get_fs_entry(std::vector<uint8_t> bytes) {
  if (this->current_task && !this->current_task.value()->is_finished()) {
    this->client.send_error("data channel is busy");
    return;
  }

  if (this->current_task) {
    this->lg->info("connection listener", "abort current task");
    this->current_task.reset();
  }

  if (!this->data_channel_client) {
    this->client.send_error("no data channel connection");
    return;
  }

  if (bytes.size() == 0) {
    this->client.send_error("required filesystem entry path");
    return;
  }

  std::filesystem::path path;
  try {
    path = this->parse_path(bytes);
  } catch (std::exception &ex) {
    this->client.send_error("invalid filesystem path");
    return;
  }

  auto data_channel_client = this->data_channel_client.value();

  this->lg->info("connection listener", "crate new filesystem task");
  this->current_task.emplace(new fs_task(this->lg, data_channel_client, path));

  this->lg->info("connection listener", "run new filesystem task");
  this->current_task.value()->run();
  this->client.send_ok();
}

std::filesystem::path connection::parse_path(std::vector<uint8_t> bytes) {
  std::string path_as_string(reinterpret_cast<char *>(bytes.data()),
                             bytes.size());
  boost::trim_right(path_as_string);

  return path_as_string;
}

void connection::handle_reconnect_data_channel() {
  try {
    this->lg->info("connection listener", "bind new data channel listener");
    this->data_channel_listener = tcp_listener(this->server_ip_addr, 0);
  } catch (std::exception &ex) {
    this->client.send_error("failed to reconnect data channel");
    return;
  }

  uint16_t port;
  try {
    port = this->data_channel_listener->get_port();
  } catch (std::exception &ex) {
    this->client.send_error("failed to reconnect data channel");
    return;
  }

  this->client.send_port(port);

  tcp_client *data_channel_client;
  try {
    this->lg->info("connection listener", "accept data channel connection");

    data_channel_client =
        new tcp_client(data_channel_listener.value().accept_client());
  } catch (std::exception &ex) {
    this->client.send_error("failed to reconnect data channel");
    return;
  }

  this->data_channel_client.emplace(data_channel_client);

  if (this->current_task) {
    current_task.reset();
  }
}
