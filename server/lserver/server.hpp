#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include "../lnet/ip_addr.hpp"
#include "handler.hpp"
#include "../lnet/tcp_listener.hpp"

class server {
public:
  server(ip_addr addr, uint16_t port, size_t max_connections_count = 10);

  server() = delete;
  server(const server &) = delete;

  void listen_clients();

private:
  tcp_listener listener;

  size_t max_connections_count;
  std::vector<std::thread> threads;
  ip_addr ip;

  void check_threads();
};
