#include "tcp_listener.hpp"

#include <stdexcept>

tcp_listener::tcp_listener(uint16_t port, uint32_t address) {
  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    throw std::runtime_error(
        "Ошибка инициализации сокета\nхорошо бы сделать "
        "свою ошибку\n");
  }
  //   std::memset(&(this->address), 0, sizeof(this->address));
  this->address.sin_family = AF_INET;
  this->address.sin_addr.s_addr = address;
  this->address.sin_port = htons(port);

  // если будут проблемы чекни тут на всякий))
  if (bind(server_fd, (struct sockaddr*)&(this->address),
           sizeof(this->address)) == -1) {
    throw std::runtime_error(
        "Ошибка привязки сокета к адресу сервера\nхорошо бы сделать "
        "свою ошибку\n");
  }
}