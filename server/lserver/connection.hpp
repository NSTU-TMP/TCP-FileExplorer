#pragma once

#include "info_client.hpp"
#include "ip_addr.hpp"
#include "logger.hpp"
#include "task.hpp"
#include "tcp_client.hpp"
#include "tcp_listener.hpp"
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <cstdint>
#include <filesystem>

class connection {
public:
  connection(boost::shared_ptr<logger>, ip_addr _server_ip_addr,
             tcp_client _tcp_client);

  void listen();

private:
  void handle_request(info_request_type request_type,
                      std::vector<uint8_t> bytes);

  void check_task_for_error();

  void handle_get_fs_entry(std::vector<uint8_t> bytes);
  std::filesystem::path parse_path(std::vector<uint8_t> bytes);

  void handle_reconnect_data_channel();

  boost::shared_ptr<logger> lg;

  ip_addr server_ip_addr;

  info_client client;

  std::optional<tcp_listener> data_channel_listener;
  std::optional<boost::shared_ptr<tcp_client>> data_channel_client;

  std::optional<boost::scoped_ptr<task>> current_task;
};
