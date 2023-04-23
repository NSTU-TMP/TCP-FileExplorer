#include "handler.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

void handler::operator()(int client_fd) {
  char buffer[1024];
  int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
  if (bytes_received == -1) {
    std::cerr << "Failed to receive message from client" << std::endl;
  } else {
    // выводим сообщение клиента
    std::cout << "Received message from client: "
              << std::string(buffer, bytes_received) << std::endl;

    // отправляем ответ клиенту
    std::string message = "Hello, client!";
    if (send(client_fd, message.c_str(), message.size(), 0) == -1) {
      std::cerr << "Failed to send message to client" << std::endl;
    }
  }
  close(client_fd);
}
