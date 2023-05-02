#include "info_client.hpp"
#include "logger.hpp"
#include "messages_types.hpp"
#include <string>
#include <utility>

info_client::info_client(boost::shared_ptr<class logger> logger,
                         tcp_client _client)
    : lg(logger), client(std::move(_client)) {}

std::string bytes_to_string(std::vector<uint8_t> &bytes) {
  return std::string(bytes.begin(), bytes.end());
}

void info_client::send_pong(std::vector<uint8_t> &&bytes) const {
  std::vector<uint8_t> response{
      info_response_into_byte(info_response_type::OK)};
  response.insert(response.end(), bytes.begin(), bytes.end());

  this->client.send(response);
  this->lg->info("info client", "send pong: " + bytes_to_string(bytes));
}

void info_client::send_port(uint16_t port) const {
  std::vector<uint8_t> bytes{info_response_into_byte(info_response_type::OK)};
  bytes.push_back(static_cast<uint8_t>((port >> 8) & 0xFF));
  bytes.push_back(static_cast<uint8_t>(port & 0xFF));

  this->client.send(bytes);
  this->lg->info("info client", "send port: " + std::to_string(port));
}

void info_client::send_error() const {
  uint8_t respone_type = info_response_into_byte(info_response_type::ERROR);
  std::vector<uint8_t> bytes{respone_type};

  this->lg->info("info client", "send error: " + bytes_to_string(bytes));
  this->client.send(bytes);
}

void info_client::send_error(std::string &&message) const {
  uint8_t respone_type = info_response_into_byte(info_response_type::ERROR);

  std::vector<uint8_t> bytes{respone_type};
  bytes.insert(bytes.end(), message.begin(), message.end());

  this->lg->info("info client", "send error: " + message);
  this->client.send(bytes);
}

std::optional<std::pair<info_request_type, std::vector<uint8_t>>>
info_client::read() const {
  std::vector<uint8_t> data;
  std::vector<uint8_t> buffer(BUFFER_SIZE);

  while (true) {
    size_t bytes_count = this->client.read_chunk(buffer, false);

    if (bytes_count == 0) {
      break;
    }

    data.insert(data.end(), buffer.begin(), buffer.begin() + bytes_count);
  }

  if (data.size() == 0) {
    return std::optional<std::pair<info_request_type, std::vector<uint8_t>>>();
  }

  auto request_type = byte_into_info_request(data[0]);

  std::vector bytes(data.begin() + 1, data.end());

  this->lg->info("info client",
                 "request has been read: " + info_request_to_string(request_type) +
                     " " + bytes_to_string(bytes));

  return std::optional(std::pair(request_type, bytes));
}
