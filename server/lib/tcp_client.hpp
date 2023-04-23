#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <unistd.h>
#include <vector>

class tcp_client {
public:
  tcp_client(uint32_t _fd) : fd(_fd) {}

  tcp_client(tcp_client &&other) noexcept : fd(other.fd) { other.fd = 0; }
  tcp_client(tcp_client &) = delete;

  tcp_client &operator=(tcp_client &&other) noexcept {
    this->fd = other.fd;
    other.fd = 0;

    return *this;
  }
  tcp_client &operator=(tcp_client const &other) = delete;

  ~tcp_client() {
    if (this->fd != 0) {
      close(this->fd);
    }
  }

  template <size_t S> void send(std::array<uint8_t, S> bytes) const {
    ssize_t writed_bytes_count = write(this->fd, bytes.data(), S);

    if (writed_bytes_count != bytes.size()) {
      throw std::runtime_error("failed to send all bytes");
    }
  }

  template <size_t S> size_t handle(std::array<uint8_t, S> &buf) const {
    if (this->fd == 0) {
      throw std::runtime_error("method has been called on moved tcp_client");
    }

    ssize_t readed_bytes_count = read(this->fd, buf.data(), S);

    if (readed_bytes_count < 0) {
      throw std::runtime_error("failed to read bytes");
    }

    return readed_bytes_count;
  }

private:
  uint32_t fd;
};
