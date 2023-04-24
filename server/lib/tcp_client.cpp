#include "tcp_client.hpp"

tcp_client::tcp_client(uint32_t _fd) : fd(_fd) {}

tcp_client::tcp_client(tcp_client&& other) noexcept : fd(other.fd) {
  other.fd = 0;
}

tcp_client& tcp_client::operator=(tcp_client&& other) noexcept {
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
  ssize_t writed_bytes_count = write(this->fd, bytes.data(), bytes.size());

  if (writed_bytes_count != bytes.size()) {
    throw std::runtime_error("failed to send all bytes");
  }
}

size_t tcp_client::read_chunk(std::vector<uint8_t>& buf) const {
  if (this->fd == 0) {
    throw std::runtime_error("method has been called on moved tcp_client");
  }

  ssize_t read_bytes_count = 0;

  read_bytes_count = read(this->fd, buf.data(), 1024);
  // }

  if (read_bytes_count < 0) {
    throw std::runtime_error("failed to read bytes");
  }

  return read_bytes_count;
}