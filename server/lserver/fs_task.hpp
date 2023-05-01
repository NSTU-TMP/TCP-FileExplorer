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

class fs_task {
public:
  fs_task(boost::weak_ptr<tcp_client> _data_client, std::string _path);

  fs_task(fs_task &other) = delete;
  fs_task &operator=(fs_task &other) = delete;
  fs_task(fs_task &&other) = delete;

  void run();
  bool is_finished();
  const std::string get_error_message() const;

  bool is_error_happend();

private:
  const char DIRS_DELIM = '\n';
  const size_t BUFFER_SIZE = 1024;

  void send_fs_entry();

  void send_dir(boost::filesystem::path path);
  void try_to_send_dir(boost::filesystem::path path);

  void send_file(boost::filesystem::path path);
  void try_to_send_file(boost::filesystem::path path);

  boost::optional<boost::thread> thr;
  boost::weak_ptr<tcp_client> data_client;

  std::string path;
  std::string error_message;

  bool error_happend = false;
  bool finished = false;

  boost::shared_ptr<tcp_client> check_connection();
};
