#include "handler.hpp"

#include <arpa/inet.h>
#include <array>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

#include "message_type.h"

handler::handler(tcp_client _client) : client(std::move(_client)) {}

void handler::handle() {
  while (true) {
    std::vector<uint8_t> data;
    std::array<uint8_t, 1024> buf;

    while (true) {
      size_t read_bytes_count = this->client.handle(buf);
      data.insert(data.end(), buf.begin(), buf.begin() + read_bytes_count);
      
      if (read_bytes_count < 1024) {
        break;
      }
    }

    std::array<uint8_t, 5> msg = {'h', 'e', 'l', 'l', 'o' };
    this->client.send(msg);
  }
}
