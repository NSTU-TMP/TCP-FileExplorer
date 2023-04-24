#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstdint>
#include <string>

struct ip_addr {
 public:
  ip_addr(std::string _str) : str(_str) {}
  ip_addr(ip_addr &other) : str(other.str) {}

  in_addr as_in_addr();

 private:
  std::string str;
};
