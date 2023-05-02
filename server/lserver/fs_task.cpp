#include "fs_task.hpp"
#include "messages_types.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>

fs_task::fs_task(boost::shared_ptr<class logger> _logger,
                 boost::weak_ptr<tcp_client> _conn, std::filesystem::path _path)
    : lg(_logger), task{_conn}, path{_path} {}

void fs_task::run() {
  if (this->thr.has_value()) {
    return;
  }

  this->thr = std::thread([this]() {
    try {
      this->send_fs_entry();
    } catch (const fs_task_error &ex) {
      this->error.emplace(ex.what());
    } catch (const std::exception &ex) {
      this->lg->info("filesystem task",
                     std::string("task execution error: ") + ex.what());

      this->error.emplace("failed to send filesystem entry");
    }

    this->lg->info("filesystem task", "filesystem task has been finished");
    this->finished = true;
  });
}

bool fs_task::is_finished() const { return this->finished; }

const boost::optional<const std::string> fs_task::get_error() const {
  if (this->error) {
    return boost::optional<const std::string>(this->error.value());
  }

  return boost::none;
}

void fs_task::send_fs_entry() {
  if (std::filesystem::is_directory(this->path)) {
    this->send_dir();
    return;
  } else if (std::filesystem::is_regular_file(this->path)) {
    this->send_file();
    return;
  }

  throw fs_task_error("unknown filesystem entry");
}

void fs_task::send_dir() {
  try {
    this->try_to_send_dir();
  } catch (const std::filesystem::filesystem_error &ex) {
    throw fs_task_error("failed to read directory entries");
  }
}

void fs_task::try_to_send_dir() {
  auto conn = this->conn.lock();

  if (!conn) {
    this->lg->warning("filesystem task", "data client disconnected");
    throw fs_task_error("data client disconnected");
  }

  std::vector flag{
      fs_data_response_into_byte(fs_data_response_type::DIRECTORY)};
  conn->send(flag);

  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    std::string dir_path_as_str(entry.path().string());
    std::vector<uint8_t> dir_path_as_bytes(dir_path_as_str.begin(),
                                           dir_path_as_str.end());

    dir_path_as_bytes.push_back(DIRS_DELIM);

    this->lg->info("filesystem task", "send dir path");
    conn->send(dir_path_as_bytes);
  }

  this->lg->info("filesystem task", "send END_OF_SENDING_DATA");

  std::vector end(END_OF_SENDING_DATA);
  conn->send(end);
}

void fs_task::send_file() {
  try {
    this->try_to_send_file();
  } catch (const std::filesystem::filesystem_error &ex) {
    throw fs_task_error("failed to read file");
  }
}

void fs_task::try_to_send_file() {
  auto conn = this->conn.lock();

  if (!conn) {
    throw fs_task_error("data client disconnected");
  }

  std::ifstream file(path, std::ifstream::binary);
  if (!file.is_open()) {
    throw fs_task_error("failed to open file");
  }

  std::vector<uint8_t> bytes(BUFFER_SIZE);

  std::vector flag{fs_data_response_into_byte(fs_data_response_type::FILE)};
  conn->send(flag);

  while (!file.eof()) {
    file.read(reinterpret_cast<char *>(bytes.data()), BUFFER_SIZE);
    const size_t readed_bytes_count = file.gcount();

    if (readed_bytes_count == 0) {
      break;
    }

    std::vector chunk(bytes.begin(), bytes.begin() + readed_bytes_count);
    conn->send(chunk);

    this->lg->info("filesystem task", "send file chunk, size = " +
                                          std::to_string(readed_bytes_count));
  }

  this->lg->info("filesystem task", "send END_OF_SENDING_DATA");

  std::vector end(END_OF_SENDING_DATA);
  conn->send(end);
}
