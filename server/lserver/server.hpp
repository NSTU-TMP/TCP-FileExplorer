#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include "../lnet/ip_addr.hpp"
#include "../lnet/tcp_listener.hpp"
#include "connection.hpp"
#include "logger.hpp"

class server {
public:
  server(boost::shared_ptr<logger> _logger, ip_addr addr, uint16_t port,
         size_t max_connections_number = 10);

  server() = delete;
  server(const server &) = delete;

  void listen_clients();

private:
  tcp_listener listener;

  size_t max_connections_number;
  std::vector<std::thread> threads;

  ip_addr ip;
  uint16_t port;

  boost::shared_ptr<logger> lg;

  void check_threads();
};
