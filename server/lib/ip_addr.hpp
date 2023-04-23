#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

struct ip_addr {
public:
  ip_addr(std::string _str) : str(_str) {}

  in_addr as_in_addr();

private:
  std::string str;
  in_addr addr;
};
