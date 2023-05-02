#pragma once

#include "../lnet/tcp_client.hpp"
#include "logger.hpp"
#include "messages_types.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <cinttypes>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

class info_client {
public:
  info_client(boost::shared_ptr<logger> logger, tcp_client _client);

  void send_pong(std::vector<uint8_t> &&bytes) const;
  void send_port(uint16_t port) const;
  void send_error() const;
  void send_error(std::string &&message) const;

  std::optional<std::pair<info_request_type, std::vector<uint8_t>>>
  read() const;

private:
  const size_t BUFFER_SIZE = 4096;

  tcp_client client;

  boost::shared_ptr<logger> lg;
};
