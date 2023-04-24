#pragma once
#include <filesystem>
#include <optional>

#include "ip_addr.hpp"
#include "tcp_client.hpp"
namespace fs = std::filesystem;

class handler {
 public:
  handler(tcp_client client);

  void handle(ip_addr server_ip);

 private:
  void send_data(tcp_client &data_sender, std::vector<uint8_t> readed_data);
  void parse_dir(tcp_client &data_sender, fs::path path_obj);
  void parse_file(tcp_client &data_sender, fs::path path_obj);
  const size_t BUFFER_SIZE = 1024;
  tcp_client client;
  std::optional<tcp_client> data_sender;
};
