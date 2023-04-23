#include "ip_addr.hpp"
#include <netinet/in.h>

in_addr ip_addr::as_in_addr() {
  in_addr addr;
  inet_pton(AF_INET, str.data(), &addr);

  return addr;
}
