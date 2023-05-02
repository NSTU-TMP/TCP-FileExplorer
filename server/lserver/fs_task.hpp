#pragma once

#include <string.h>

#include <boost/cstdint.hpp>
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/thread.hpp>
#include <exception>
#include <stdexcept>

#include "../lnet/tcp_client.hpp"
#include "message_type.hpp"
#include "task.hpp"

class fs_task : public task {
public:
  fs_task(boost::weak_ptr<tcp_client> _conn, std::string _path);

  void run() override;

  bool is_finished() const override;

  const boost::optional<const std::string> get_error() const override;

private:
  const char DIRS_DELIM = '\n';
  const size_t BUFFER_SIZE = 1024;

  void send_fs_entry();

  void send_dir(boost::filesystem::path path);
  void try_to_send_dir(boost::filesystem::path path);

  void send_file(boost::filesystem::path path);
  void try_to_send_file(boost::filesystem::path path);

  boost::optional<boost::thread> thr;
  boost::optional<std::string> error;
  std::string path;

  bool finished = false;
};
