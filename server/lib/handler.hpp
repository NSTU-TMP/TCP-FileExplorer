#pragma once

class handler {
 private:
 public:
  handler() {}
  void operator()(int client_fd);
};
