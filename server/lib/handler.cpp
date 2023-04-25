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

namespace fs = std::filesystem;

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

      readed_data.insert(readed_data.end(), buf.begin(),
                         buf.begin() + read_bytes_count);

      if (read_bytes_count < 1024) {
        // TODO
        break;
      }
    }

    if (readed_data.size() == 0)
      continue;

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
    thread
    this->handle_get_data_request(this->data_sender.value(), readed_data);
    return;
  }

  if (request == PING) {
    this->handle_ping_request();
    return;
  }

  throw std::runtime_error("TODO");
}

void handler::handle_ping_request() { throw std::runtime_error("TODO"); }

void handler::handle_get_data_request(tcp_client &data_sender,
                                      std::vector<uint8_t> readed_data) {
  std::string path_str(reinterpret_cast<char *>(readed_data.data()),
                       readed_data.size());

  fs::path path_obj(path_str);

  if (fs::is_directory(path_obj)) {
    this->parse_dir(data_sender, path_obj);
  }

  if (fs::is_regular_file(path_obj)) {
    this->parse_file(data_sender, path_obj);
  }

  throw std::runtime_error("unknown filesystem entry");
}

void handler::parse_dir(tcp_client &data_sender, fs::path path_obj) {
  for (const auto &entry : fs::directory_iterator(path_obj)) {
    std::string path_as_str = entry.path().string();
    std::vector<uint8_t> path_as_bytes(path_as_str.begin(), path_as_str.end());

    data_sender.send(path_as_bytes);
  }

  data_sender.send(END_OF_SENDING_DATA);
}

void handler::parse_file(tcp_client &data_sender, fs::path path_obj) {
  std::ifstream file(path_obj);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file");
  }

  std::vector<uint8_t> bytes(BUFFER_SIZE);

  while (file) {
    file.read(reinterpret_cast<char *>(bytes.data()), BUFFER_SIZE);
    const size_t readed_bytes_count = file.gcount();

    if (readed_bytes_count > 0) {
      bytes.resize(readed_bytes_count);
      data_sender.send(bytes);
    }
  }

  data_sender.send(END_OF_SENDING_DATA);
}
