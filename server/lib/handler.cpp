#include "handler.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "message_type.h"

void handler::operator()(int client_fd) {
  int message[1];
  int bytes_received = recv(client_fd, message, sizeof(message), 0);

  if (message[0] == EXIT) {
    close(client_fd);
    return;
  }
  if (message[0] == PING) {
    //...
    return;
  }
  if (message[0] == GET_DATA) {
    char buffer[1024];
    //...
    return;
  }

  // if (bytes_received == -1) {
  //   std::cerr << "Failed to receive message from client" << std::endl;
  // } else {
  //   // выводим сообщение клиента
  //   std::cout << "Received message from client: "
  //             << std::string(buffer, bytes_received) << std::endl;

  //   // отправляем ответ клиенту
  //   std::string message = "Hello, client!";
  //   if (send(client_fd, message.c_str(), message.size(), 0) == -1) {
  //     std::cerr << "Failed to send message to client" << std::endl;
  //   }
  // }

  // close(client_fd);
}
