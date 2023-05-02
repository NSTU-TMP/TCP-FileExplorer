#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../lnet/ip_addr.hpp"
#include "../lnet/tcp_client.hpp"
#include "../lnet/tcp_listener.hpp"
#include "fs_task.hpp"
#include "info_client.hpp"
#include "message_type.hpp"
#include "task.hpp"

class handler {
public:
  handler(tcp_client client, ip_addr server_ip);

  void handle();

private:
  void check_task_for_error();

  void handle_request(std::vector<uint8_t> readed_data);

  void handle_data_channel_reconnect();
  void generate_port();

  void handle_get_data(std::vector<uint8_t> readed_data);

  info_client client;

  ip_addr server_ip;
  uint16_t port;

  tcp_listener data_connection_listener;

  boost::optional<boost::scoped_ptr<task>> current_task;
  boost::shared_ptr<tcp_client> data_client;
};
