#include "server.hpp"
#include "handler.hpp"
#include "ip_addr.hpp"

server::server(ip_addr address, uint16_t port, int _max_connections_count)
    : listener{address, port}, max_connections_count(_max_connections_count) {}

void server::check_threads() {
  for (int i = 0; i < this->threads.size(); i++) {
    if (!this->threads.at(i).joinable()) {
      this->threads.erase(this->threads.begin() + i);
      i = i > 0 ? i - 1 : 0;
    }
  }
}

void server::listen_clients() {
  std::cout << "Слушаю дорогой\n";
  while (true) {
    this->check_threads();

    if (threads.size() < this->max_connections_count) {
      tcp_client client = this->listener.accept_client();

      std::thread thr([client = std::move(client)]() mutable {
        handler hdlr(std::move(client));
        hdlr.handle();
      });

      threads.push_back(std::move(thr));
    }
  }
}
