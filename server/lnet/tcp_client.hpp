#pragma once

#include <unistd.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

class tcp_client {
public:
  tcp_client(uint32_t _fd);

  tcp_client(tcp_client &&other) noexcept;
  tcp_client(tcp_client &) = delete;
  tcp_client &operator=(tcp_client const &other) = delete;

  tcp_client &operator=(tcp_client &&other) noexcept;

  ~tcp_client();
  void send(std::vector<uint8_t> &bytes) const;

  size_t read_chunk(std::vector<uint8_t> &buf, bool blocked = false) const;

private:
  uint32_t fd;
};

size_t sock_send(int fd, std::vector<uint8_t> bytes);
