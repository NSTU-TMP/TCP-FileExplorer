#include "info_client.hpp"

info_client::info_client(tcp_client _client) : client(std::move(_client)) {}

void info_client::send_ping() const {
  throw std::runtime_error("no impl of send_ping");
}

void info_client::send_error(response_type error) const {
  throw std::runtime_error("no impl of send_error");
}

void info_client::send_error_message(std::string message) const {
  throw std::runtime_error("no impl of send_error_message");
}

size_t info_client::read_chunk(std::vector<uint8_t>& buf) const {
  return this->client.read_chunk(buf);
}
