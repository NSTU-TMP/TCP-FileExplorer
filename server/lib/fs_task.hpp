#include "tcp_client.hpp"

#include <boost/cstdint.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/thread.hpp>
#include <cstdint>
#include <string.h>

class fs_task {
public:
  fs_task(boost::weak_ptr<tcp_client> _data_client, std::string _path,
          std::vector<uint8_t> _readed_data);

  fs_task(fs_task &other) = delete;
  fs_task &operator=(fs_task &other) = delete;

  bool is_finished();

private:
  const size_t BUFFER_SIZE = 1024;

  void send_fs_entry();

  void send_dir(boost::filesystem::path path);
  void try_to_send_dir(boost::filesystem::path path);

  void send_file(boost::filesystem::path path);
  void try_to_send_file(boost::filesystem::path path);

  boost::weak_ptr<tcp_client> data_client;
  std::vector<uint8_t> readed_data;
  std::string path;
  boost::thread thr;
};
