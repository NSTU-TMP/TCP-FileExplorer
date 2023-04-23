#include "server.hpp"

// #include "handler.hpp"

server::server(uint16_t port, uint32_t address, int max_connections_count) {
  this->max_connections_count = max_connections_count;

  this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->server_fd == -1) {
    std::cerr << "Failed to create socket\n";
    throw std::runtime_error(
        "Ошибка инициализации сокета\nхорошо бы сделать "
        "свою ошибку\n");
  }
  //   std::memset(&(this->address), 0, sizeof(this->address));
  this->address.sin_family = AF_INET;
  this->address.sin_addr.s_addr = address;
  this->address.sin_port = htons(port);

  // если будут проблемы чекни тут на всякий))
  if (bind(this->server_fd, (struct sockaddr*)&(this->address),
           sizeof(this->address)) == -1) {
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
    if (threads.size() < this->max_connections_count) {
      int client_fd = accept(server_fd, NULL, NULL);
      if (client_fd == -1) {
        throw std::runtime_error("Failed to accept connection");
        continue;
      }

      std::thread t(this->client_handler, client_fd);

      threads.push_back(std::move(t));
    }
  }
}
