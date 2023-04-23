#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include "handler.hpp"

class server {
 private:
  struct sockaddr_in address;
  int server_fd;
  server() = delete;
  server(const server&) = delete;
  int max_connections_count;
  std::vector<std::thread> threads;
  handler client_handler;
  void check_threads();

 public:
  server(uint16_t port, int max_connections_count = 100);
  ~server();
  void listen_clients();
};
