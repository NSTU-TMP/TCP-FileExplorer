#include "handler.hpp"

#include <arpa/inet.h>
#include <filesystem>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include "message_type.hpp"
#include "tcp_listener.hpp"

handler::handler(tcp_client _client) : client(std::move(_client)) {}

void handler::handle(ip_addr server_ip) {
  tcp_listener listener(server_ip, 0);

  uint16_t port = listener.get_port();
  std::cout << port;
  std::array<uint8_t, 2> byte_port = {static_cast<uint8_t>((port >> 8) & 0xFF),
                                      static_cast<uint8_t>(port & 0xFF)};

  std::vector<uint8_t> readed_data;
  std::vector<uint8_t> buf(1024);

  while (true) {


    while (true) {
      size_t read_bytes_count = this->client.read_chunk(buf);

      if (read_bytes_count == 0) {
        break;
      }

      readed_data.insert(readed_data.end(), buf.begin(),
                         buf.begin() + read_bytes_count);
    }

    if (readed_data.size() == 0) {
      continue;
    }
    // HANDLE

    readed_data.clear();
  }
}

void handler::handle_request(std::vector<uint8_t> readed_data) {
  request_type request = request_type_from_byte(readed_data[0]);

  if (GET_PORT != request && !this->data_sender.has_value()) {
    // нет запроса на получение порта для передачи данных с сервера
    // и нет самого подключения
    // TODO
    return;
  }

  if (GET_DATA == request) {
    // this->handle_get_data_request(this->data_sender.value(), readed_data);
    return;
  }

  if (request == PING) {
    this->handle_ping_request();
    return;
  }

  throw std::runtime_error("TODO");
}

void handler::handle_ping_request() { throw std::runtime_error("TODO"); }
