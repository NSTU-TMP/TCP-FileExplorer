#include "server.hpp"

// #include "handler.hpp"

server::server(uint16_t port, int max_connections_count) {
  this->max_connections_count = max_connections_count;

  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    std::cerr << "Failed to create socket\n";
    throw std::runtime_error(
        "Ошибка инициализации сокета\nхорошо бы сделать "
        "свою ошибку\n");
  }
  //   std::memset(&(this->address), 0, sizeof(this->address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
    std::cerr << "Failed to bind socket to address\n";
    throw std::runtime_error(
        "Ошибка привязки сокета к адресу сервера\nхорошо бы сделать "
        "свою ошибку\n");
  }
}

void server::check_threads() {
  for (int i = 0; i < this->threads.size(); i++) {
    if (!this->threads.at(i).joinable()) {
      this->threads.erase(this->threads.begin() + i);
      i = i > 0 ? i - 1 : 0;
    }
  }
}

server::~server() {}

void server::listen_clients() {
  if (listen(this->server_fd, this->max_connections_count) == -1) {
    throw std::runtime_error("Failed to start listening for connections\n");
  }
  int index = 0;
  std::cout << "Слушаю дорогой\n";
  while (true) {
    this->check_threads();
    // принимаем новое соединение
    if (threads.size() < this->max_connections_count) {
      int client_fd = accept(server_fd, NULL, NULL);
      if (client_fd == -1) {
        throw std::runtime_error("Failed to accept connection");
        continue;
      }

      // std::cout << "я в тренде\n";

      std::thread t(this->client_handler, client_fd);

      threads.push_back(std::move(t));
    }
  }
}

// обрабатываем новое соединение в отдельном потоке
// std::thread t([&]() {
//   char buffer[1024];
//   int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
//   if (bytes_received == -1) {
//     std::cerr << "Failed to receive message from client" << std::endl;
//   } else {
//     // выводим сообщение клиента
//     std::cout << "Received message from client: "
//               << std::string(buffer, bytes_received) << std::endl;

//     // отправляем ответ клиенту
//     std::string message = "Hello, client!";
//     if (send(client_fd, message.c_str(), message.size(), 0) == -1) {
//       std::cerr << "Failed to send message to client" << std::endl;
//     }
//   }
//   close(client_fd);
// });