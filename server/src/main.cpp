#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include "server.hpp"

int main() {
  uint16_t port = 4460;
  int flag = 1;
  while (flag) {
    try {
      std::cout << port << "\n";
      server s(ip_addr("127.0.0.1"), port);
      s.listen_clients();
      flag = 0;
    } catch (std::runtime_error &ex) {
      port++;
    }
  }
}
