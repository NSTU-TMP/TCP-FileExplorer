#include "handler.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
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
  std::array<uint8_t, 2> byte_port = {(port >> 8) & 0xFF, port & 0xFF};

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
    if (readed_data.size() == 0) continue;

    request_type request = request_type_from_byte(readed_data[0]);
    if (GET_PORT != request && !this->data_sender.has_value()) {
      // нет запроса на получение порта для передачи данных с сервера
      // и нет самого подключения
      // TODO
    } else {
      if (GET_DATA == request) {
        this->send_data(this->data_sender.value(), readed_data);
      } else if (1) {
      }
    }
    readed_data.clear();
  }
}

void handler::send_data(tcp_client& data_sender,
                        std::vector<uint8_t> readed_data) {
  std::string path_str(reinterpret_cast<char*>(readed_data.data()),
                       readed_data.size());
  fs::path path_obj(path_str);
  if (fs::is_directory(path_obj)) {
    this->parse_dir(data_sender, path_obj);
  } else {
    this->parse_file(data_sender, path_obj);
  }

  data_sender.send(END_OF_SENDING_DATA);
}

void handler::parse_dir(tcp_client& data_sender, fs::path path_obj) {
  for (const auto& entry : fs::directory_iterator(path_obj)) {
    std::string path_str = entry.path().string();
    std::vector<uint8_t> path_vec(path_str.begin(), path_str.end());
    data_sender.send(path_vec);
  }
}

void handler::parse_file(tcp_client& data_sender, fs::path path_obj) {
  std::ifstream file(path_obj);
  if (!file.is_open()) {
    // TODO
    throw std::runtime_error("cant open file");
  } else {
    std::vector<uint8_t> byte_vector(BUFFER_SIZE);
    while (file) {
      file.read(reinterpret_cast<char*>(byte_vector.data()), BUFFER_SIZE);
      const size_t bytes_read = file.gcount();
      if (bytes_read > 0) {
        byte_vector.resize(bytes_read);
        data_sender.send(byte_vector);
      }
    }
  }
}
