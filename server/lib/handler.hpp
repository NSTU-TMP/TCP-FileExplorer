#pragma once

#include "tcp_client.hpp"

class handler {
public:
  handler(tcp_client client);

  void handle();

private:
  tcp_client client;
};
