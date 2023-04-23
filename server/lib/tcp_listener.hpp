#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
class tcp_listener {
 private:
  struct sockaddr_in address;
  int server_fd;
  tcp_listener() = delete;
  tcp_listener(const tcp_listener&) = delete;

 public:
  tcp_listener(uint16_t port, uint32_t address);
  void accept();
};
