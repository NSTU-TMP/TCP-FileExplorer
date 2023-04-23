#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
class tcp_listener {
 private:
  struct sockaddr_in address;
  int server_fd;
  tcp_listener() = delete;
  tcp_listener(const tcp_listener&) = delete;

 public:
  tcp_listener(int server_fd, struct sockaddr_in address);
  void accept_client();
  //   static void asdsa();
};
