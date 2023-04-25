#pragma once
#include <filesystem>
#include <optional>

#include "ip_addr.hpp"
#include "tcp_client.hpp"

class handler {
public:
  handler(tcp_client client);

  void handle(ip_addr server_ip);

private:
  void handle_request(std::vector<uint8_t> readed_data);

  void handle_ping_request();

  tcp_client client;
  std::optional<tcp_client> data_sender;
};
