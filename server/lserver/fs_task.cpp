#include "fs_task.hpp"

namespace fs = boost::filesystem;

fs_task::fs_task(boost::weak_ptr<tcp_client> _conn, std::string _path)
    : task{_conn}, path{_path} {}

void fs_task::run() {
  if (this->thr.has_value()) {
    return;
  }

  this->thr = boost::thread([this]() {
    try {
      this->send_fs_entry();
    } catch (const std::exception &ex) {
      this->error.emplace(ex.what());
    }
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
  fs::path path_obj(this->path);

  if (fs::is_directory(path_obj)) {
    this->send_dir(path_obj);
    return;
  } else if (fs::is_regular_file(path_obj)) {
    this->send_file(path_obj);
    return;
  }

  throw std::runtime_error("unknown fs entry");
}

void fs_task::send_dir(fs::path path) {
  try {
    this->try_to_send_dir(path);
  } catch (const fs::filesystem_error &ex) {
    throw std::runtime_error("failed to read directory entries");
  }
}

void fs_task::try_to_send_dir(fs::path path) {
  auto conn = this->conn.lock();

  if (!conn) {
    throw std::runtime_error("data client disconnected");
  }

  for (const auto &entry : fs::directory_iterator(path)) {
    fs::path dir_path(entry.path());
    std::string dir_path_str(dir_path.string());

    std::vector<uint8_t> path_as_bytes(dir_path_str.begin(),
                                       dir_path_str.end());
    path_as_bytes.push_back(DIRS_DELIM);

    conn->send(path_as_bytes);
  }

  conn->send(END_OF_SENDING_DATA);
}

void fs_task::send_file(fs::path path) {
  try {
    this->try_to_send_file(path);
  } catch (const fs::filesystem_error &ex) {
    throw std::runtime_error("failed to read file");
  }
}

void fs_task::try_to_send_file(fs::path path) {
  auto conn = this->conn.lock();

  if (!conn) {
    throw std::runtime_error("data client disconnected");
  }

  fs::ifstream file(path, std::ifstream::binary);
  if (!file.is_open()) {
    throw std::runtime_error("failed to open file");
  }

  std::vector<uint8_t> bytes(BUFFER_SIZE);

  while (!file.eof()) {
    file.read(reinterpret_cast<char *>(bytes.data()), BUFFER_SIZE);
    const size_t readed_bytes_count = file.gcount();

    if (readed_bytes_count > 0) {
      conn->send(
          std::vector(bytes.begin(), bytes.begin() + readed_bytes_count));
    }
  }

  conn->send(END_OF_SENDING_DATA);
}
