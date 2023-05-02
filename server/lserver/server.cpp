#include "server.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <exception>

server::server(boost::shared_ptr<class logger> _logger, ip_addr address,
               uint16_t port, size_t _max_connections_count)
    : lg(_logger), listener{address, port}, port(port),
      max_connections_number(_max_connections_count), ip(address) {}

void server::check_threads() {
  for (size_t i = 0; i < this->threads.size(); i++) {
    if (!this->threads.at(i).joinable()) {
      this->threads.erase(this->threads.begin() + i);
      i = i > 0 ? i - 1 : 0;
    }
  }
}

void server::listen_clients() {
  lg->info("server", "start listening on " + this->ip.as_string() + ":" +
                         std::to_string(this->port));

  while (true) {
    this->check_threads();

    if (threads.size() < this->max_connections_number) {
      tcp_client client = this->listener.accept_client();
      ip_addr server_ip = this->ip;

      std::thread thr([logger = boost::shared_ptr(this->lg),
                       client = std::move(client), &server_ip]() mutable {
        try {
          connection conn(logger, server_ip, std::move(client));
          conn.listen();
        } catch (std::exception &ex) {
          logger->error("connection thread", ex.what());
        }
      });

      threads.push_back(std::move(thr));
    }
  }
}
