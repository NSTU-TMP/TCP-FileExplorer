#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../lnet/ip_addr.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../lserver/server.hpp"

int main() {
  uint16_t port = 4460;

  while (true) {
    try {
      std::cout << port << "\n";
      std::string addr_as_str("127.0.0.1");

      ip_addr addr(addr_as_str);

      server s(addr, port);
      s.listen_clients();
    } catch (std::runtime_error &ex) {
      port += 1;
    }
  }
}
