#include <signal.h>
#include <unistd.h>

#include "../lnet/ip_addr.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../lserver/console_logger.hpp"
#include "../lserver/logger.hpp"
#include "../lserver/server.hpp"

int main() {
  signal(SIGPIPE, SIG_IGN);

  boost::shared_ptr<logger> logger(new console_logger());
  uint16_t port = 4460;

  while (true) {
    try {
      std::string addr_as_str("127.0.0.1");

      ip_addr addr(addr_as_str);

      server s(logger, addr, port);
      s.listen_clients();
    } catch (std::runtime_error &ex) {
      port += 1;
    }
  }
}
