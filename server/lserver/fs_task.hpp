#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>

#include "../lnet/tcp_client.hpp"
#include "logger.hpp"
#include "messages_types.hpp"
#include "task.hpp"

class fs_task : public task {
public:
  fs_task(boost::shared_ptr<logger> _logger, boost::weak_ptr<tcp_client> _conn,
          std::filesystem::path _path);

  void run() override;

  bool is_finished() const override;

  const boost::optional<const std::string> get_error() const override;

private:
  const char DIRS_DELIM = '\n';
  const size_t BUFFER_SIZE = 4096;

  void send_fs_entry();

  void send_dir();
  void try_to_send_dir();

  void send_file();
  void try_to_send_file();

  bool finished = false;
  std::optional<std::thread> thr;
  std::optional<std::string> error;
  std::filesystem::path path;

  boost::shared_ptr<logger> lg;
};

class fs_task_error : public std::runtime_error {
  using std::runtime_error::runtime_error;
};
