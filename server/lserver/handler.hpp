#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
// #include <boost/optional.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include "fs_task.hpp"
#include "info_client.hpp"
#include "ip_addr.hpp"
#include "message_type.hpp"
#include "tcp_client.hpp"
#include "tcp_listener.hpp"
class handler {
 public:
  handler(tcp_client client, ip_addr server_ip);

  void handle();

 private:
  void handle_request(std::vector<uint8_t> readed_data);
  void generate_port();
  info_client client;
  ip_addr server_ip;
  uint16_t port;
  tcp_listener data_connection_listener;
  std::optional<fs_task> task;
  boost::shared_ptr<tcp_client> data_client;
};
