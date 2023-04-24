#pragma once

#include <unistd.h>

#include "ip_addr.hpp"
#include "tcp_client.hpp"

class tcp_listener {
  const uint32_t SOCK_BUFF_SIZE = 3;

 public:
  tcp_listener(ip_addr ip_addr, uint16_t port);

  tcp_listener(tcp_listener&& other) noexcept;
  tcp_listener(tcp_listener&) = delete;

  ~tcp_listener();

  tcp_listener& operator=(tcp_listener&&) noexcept;
  tcp_listener& operator=(tcp_listener const& other) = delete;

  uint16_t get_port();

  tcp_client accept_client() const;

 private:
  uint32_t fd;
};
