#include "tcp_listener.hpp"

#include <stdexcept>

// чето надо с этим сделать
void create_client_stream(uint16_t client_port, uint32_t server_address,
                          int &client_fd_send_requests) {}

tcp_listener::tcp_listener(int server_fd, sockaddr_in address) {
  this->server_fd = server_fd;
  this->address = address;
}

void tcp_listener::accept_client() {
  int client_fd_get_requests = accept(this->server_fd, NULL, NULL);
  int client_fd_send_requests;

  //   this->address.sin_addr;
  this->address.sin_addr.s_addr;
  uint16_t client_port = 0;  // !!!!!

  //   std::string message = "Hello, client!";
  if (send(client_fd_get_requests, &client_port, sizeof(client_port), 0) ==
      -1) {
    std::cerr << "Failed to send message to client" << std::endl;
  }
}
