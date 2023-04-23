#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include "../lib/server.hpp"

int main() {
  server s(ip_addr("127.0.0.1"), 4444);

  s.listen_clients();
}
