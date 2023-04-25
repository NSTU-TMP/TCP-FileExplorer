#include "tcp_listener.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <iostream>
#include <stdexcept>

tcp_listener::tcp_listener(tcp_listener &&other) noexcept : fd(other.fd) {
  other.fd = 0;
}

tcp_listener &tcp_listener::operator=(tcp_listener &&other) noexcept {
  this->fd = other.fd;
  other.fd = 0;

  return *this;
}

tcp_listener::~tcp_listener() {
  if (this->fd != 0) {
    close(this->fd);
  }
}

bool tcp_listener::is_closed() {
  throw std::runtime_error("tcp_listener: TODO is_closed");
  return false;
}

tcp_listener::tcp_listener(ip_addr ip_addr, uint16_t port) {
  uint32_t socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    throw std::runtime_error("socket creation failed");
  }

  sockaddr_in sock_addr;
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr = ip_addr.as_in_addr();
  sock_addr.sin_port = port != 0 ? htons(port) : 0;

  if (bind(socket_fd, (sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
    close(socket_fd);
    throw std::runtime_error("socket bind failed");
  }

  if (port == 0) {
    std::cout << "port was 0 ->" << sock_addr.sin_port << "\n";
  }

  if (listen(socket_fd, SOCK_BUFF_SIZE) < 0) {
    close(socket_fd);
    throw std::runtime_error("start listening failed");
  }

  this->fd = socket_fd;
}

uint16_t tcp_listener::get_port() {
  sockaddr_in addr;
  socklen_t addrlen;
  if (getsockname(this->fd, (struct sockaddr *)&addr, &addrlen) < 0) {
    close(this->fd);
    throw std::runtime_error("Error getting socket name");
  }
  return ntohs(addr.sin_port);
}

tcp_client tcp_listener::accept_client() const {
  if (this->fd == 0) {
    throw std::runtime_error("method has been called on moved tcp_listener");
  }

  sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(sockaddr_in);

  uint32_t client_fd =
      accept((int)this->fd, (struct sockaddr *)&client_addr, &client_addr_len);

  if (client_fd < 0) {
    throw std::runtime_error("tcp_client: Failed client accept");
  }

  return tcp_client(client_fd);
}
