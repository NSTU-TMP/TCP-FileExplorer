#include "ip_addr.hpp"

in_addr ip_addr::as_in_addr() {
  in_addr addr;
  inet_pton(AF_INET, str.data(), &addr);

  return addr;
}

std::string ip_addr::as_string() { return std::string(this->str); }
