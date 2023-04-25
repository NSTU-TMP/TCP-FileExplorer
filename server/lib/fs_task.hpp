#include "tcp_client.hpp"
#include <mutex>
#include <utility>

struct data_client {
public:
  data_client(tcp_client _client);

private:
  tcp_client client;
  std::mutex mtx;
};

class fs_task {
public:
  fs_task(data_client &_data_client);

  fs_task(fs_task &other) = delete;
  fs_task &operator=(fs_task &other) = delete;

  void run();

  bool is_finished();

private:
  tcp_client &data_client;
};
