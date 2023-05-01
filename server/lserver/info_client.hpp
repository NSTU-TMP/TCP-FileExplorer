#include "../lnet/tcp_client.hpp"
#include "message_type.hpp"
class info_client {
private:
  tcp_client client;

public:
  info_client(tcp_client _client);

  void send_pong();

  void send_error(response_type error);
  void send_error_with_message(response_type error, std::string message);
  void send_port(uint16_t port);

  ssize_t read_chunk(std::vector<uint8_t> &buf);
};
