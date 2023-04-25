#include "fs_task.hpp"
#include <thread>

data_client::data_client(tcp_client _client) : client(std::move(_client)) {}

// fs_task::fs_task(tcp_client &_data_client) : data_client(_data_client) {}

void fs_task::run() {
  std::thread task_thread([]() {});
}
