#include "tcp_client.hpp"

#include <fcntl.h>
#include <sys/socket.h>

tcp_client::tcp_client(uint32_t _fd) : fd(_fd) {
  int flags = fcntl((int)this->fd, F_GETFL, 0);
  fcntl((int)this->fd, F_SETFL, flags | O_NONBLOCK);
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
    close((int)this->fd);
  }
}

void tcp_client::send(std::vector<uint8_t> bytes) {
  size_t writed_bytes_count = sock_send((int)this->fd, bytes);

  if (writed_bytes_count != bytes.size()) {
    throw std::runtime_error("failed to send all bytes");
  }
}

size_t sock_send(int fd, std::vector<uint8_t> bytes) {
  while (true) {
    ssize_t writed_bytes_count = write(fd, bytes.data(), bytes.size());

    if (writed_bytes_count >= 0) {
      return (size_t)writed_bytes_count;
    }

    if (errno == EWOULDBLOCK) {
      continue;
    }

    throw std::runtime_error("undefined socket error, errno = " +
                             std::to_string(errno));
  }
}

bool tcp_client::is_closed() {
  throw std::runtime_error("tcp_client: TODO is_closed");
  return false;
}

ssize_t tcp_client::read_chunk(std::vector<uint8_t> &buf, bool blocked) {
  if (this->fd == 0) {
    throw std::runtime_error("method has been called on moved tcp_client");
  }

  while (true) {
    ssize_t read_bytes_count = read((int)this->fd, buf.data(), buf.size());

    if (read_bytes_count >= 0) {
      return read_bytes_count;
    }

    if (blocked && errno == EWOULDBLOCK) {
      continue;
    }

    if (errno == EAGAIN) {
      return 0;
    }

    throw std::runtime_error("undefined socket error, errno = " +
                             std::to_string(errno));
  }
}
