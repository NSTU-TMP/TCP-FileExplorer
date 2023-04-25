#include "fs_task.hpp"

namespace fs = boost::filesystem;

fs_task::fs_task(boost::weak_ptr<tcp_client> _data_client, std::string _path)
    : data_client(_data_client),
      path(_path),
      error_message(nullptr),
      error_happend(false) {}

void fs_task::run() {
  if (!this->thr.has_value()) {
    this->thr = boost::thread([this]() {
      try {
        this->send_fs_entry();
      } catch (const std::exception &ex) {
        this->error_happend = true;
        this->error_message = ex.what();
      }
    });
  }
}

bool fs_task::is_finished() {
  return this->thr.has_value() && !this->thr.value().joinable();
}

std::string fs_task::get_error_message() { return this->error_message; }

bool fs_task::is_error_happend() { return this->error_happend; }

boost::shared_ptr<tcp_client> fs_task::check_connection() {
  auto data_connection = this->data_client.lock();
  if (!data_connection) {
    throw std::runtime_error("data client disconnected");
  }
  return data_connection;
}

void fs_task::send_fs_entry() {
  fs::path path_obj(this->path);

  if (fs::is_directory(path_obj)) {
    this->send_dir(path_obj);
  } else if (fs::is_regular_file(path_obj)) {
    this->send_file(path_obj);
  }

  throw std::runtime_error("unknown filesystem entry");
}

void fs_task::send_dir(fs::path path) {
  try {
    this->try_to_send_dir(path);
  } catch (const fs::filesystem_error &ex) {
    throw std::runtime_error("failed to read directory entries");
  }
}
#include <iostream>

void fs_task::try_to_send_dir(fs::path path) {
  auto data_connection = this->check_connection();

  for (const auto &entry : fs::directory_iterator(path)) {
    fs::path dir_path(entry.path());
    std::string dir_path_str(dir_path.string());
    std::vector<uint8_t> path_as_bytes(dir_path_str.begin(),
                                       dir_path_str.end());
    data_connection->send(path_as_bytes);
  }

  data_connection->send(END_OF_SENDING_DATA);
}

void fs_task::send_file(fs::path path) {
  try {
    this->try_to_send_file(path);
  } catch (const fs::filesystem_error &ex) {
    throw std::runtime_error("failed to read file");
  }
}

void fs_task::try_to_send_file(fs::path path) {
  auto data_connection = this->check_connection();

  fs::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("failed to open file");
  }

  std::vector<uint8_t> bytes(BUFFER_SIZE);

  while (file) {
    file.read(reinterpret_cast<char *>(bytes.data()), BUFFER_SIZE);
    const size_t readed_bytes_count = file.gcount();

    if (readed_bytes_count > 0) {
      bytes.resize(readed_bytes_count);
      data_connection->send(bytes);
    }
  }

  data_connection->send(END_OF_SENDING_DATA);
}
