#include "tcp_client.hpp"

#include <fcntl.h>

tcp_client::tcp_client(uint32_t _fd) : fd(_fd) {
  int flags = fcntl(this->fd, F_GETFL, 0);
  fcntl(this->fd, F_SETFL, flags | O_NONBLOCK);
}

tcp_client::tcp_client(tcp_client &&other) noexcept : fd(other.fd) {
  other.fd = 0;
}

tcp_client &tcp_client::operator=(tcp_client &&other) noexcept {
  this->fd = other.fd;
  other.fd = 0;

  return *this;
}

tcp_client::~tcp_client() {
  if (this->fd != 0) {
    close(this->fd);
  }
}

void tcp_client::send(std::vector<uint8_t> bytes) const {
  size_t writed_bytes_count =
      (size_t)write(this->fd, bytes.data(), bytes.size());

  if (writed_bytes_count != bytes.size()) {
    throw std::runtime_error("failed to send all bytes");
  }
}

bool tcp_client::is_closed() {
  throw std::runtime_error("tcp_client: TODO is_closed");
  return false;
}

ssize_t tcp_client::read_chunk(std::vector<uint8_t> &buf) const {
  if (this->fd == 0) {
    throw std::runtime_error("method has been called on moved tcp_client");
  }

  return read(this->fd, buf.data(), buf.size());
}
