#include "fs_task.hpp"
#include "message_type.hpp"

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <exception>
#include <stdexcept>

namespace fs = boost::filesystem;

fs_task::fs_task(boost::weak_ptr<tcp_client> _data_client, std::string _path,
                 std::vector<uint8_t> _readed_data)
    : data_client(_data_client), path(_path), readed_data(_readed_data) {
  boost::thread thr([this]() {
    try {
      this->send_fs_entry();
    } catch (const std::exception &ex) {
      // TODO
    }
  });
}

bool fs_task::is_finished() { return !this->thr.joinable(); }

void fs_task::send_fs_entry() {
  std::string path_str(reinterpret_cast<char *>(readed_data.data()),
                       readed_data.size());

  fs::path path_obj(path_str);

  if (fs::is_directory(path_obj)) {
    this->send_dir(path_obj);
  }

  if (fs::is_regular_file(path_obj)) {
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

void fs_task::try_to_send_dir(fs::path path_obj) {
  auto data_connection = this->data_client.lock();
  if (!data_connection) {
    throw std::runtime_error("data client disconnected");
  }

  for (const auto &entry : fs::directory_iterator(path_obj)) {
    std::vector<uint8_t> path_as_bytes(this->path.begin(), this->path.end());
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
  auto data_connection = this->data_client.lock();
  if (!data_connection) {
    throw std::runtime_error("data client disconnected");
  }

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
