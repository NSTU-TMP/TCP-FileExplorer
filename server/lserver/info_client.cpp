#include "info_client.hpp"

info_client::info_client(tcp_client _client) : client(std::move(_client)) {}

void info_client::send_pong() {
  std::vector<uint8_t> ping_as_bytes = {PING};
  this->client.send(ping_as_bytes);
}

void info_client::send_error(response_type error) {
  std::vector<uint8_t> error_as_bytes = {error};
  this->client.send(error_as_bytes);
}

void info_client::send_error_with_message(response_type error,
                                          std::string message) {
  std::vector<uint8_t> message_as_bytes;

  message_as_bytes.push_back(error);
  message_as_bytes.insert(message_as_bytes.end(), message.begin(),
                          message.end());

  this->client.send(message_as_bytes);
}

ssize_t info_client::read_chunk(std::vector<uint8_t> &buf) {
  return this->client.read_chunk(buf);
}

void info_client::send_port(uint16_t port) {
  std::vector<uint8_t> byte_port = {static_cast<uint8_t>((port >> 8) & 0xFF),
                                    static_cast<uint8_t>(port & 0xFF)};
  this->client.send(byte_port);
}
