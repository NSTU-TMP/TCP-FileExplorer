#pragma once

#include "tcp_client.hpp"
#include <boost/optional/optional.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/thread/detail/move.hpp>

class task {
public:
  task(boost::weak_ptr<tcp_client> _conn);

  task(task &other) = delete;
  task &operator=(task &other) = delete;

  task(task &&other) = delete;
  task &operator=(task &&other) = delete;

  virtual void run() = 0;

  virtual bool is_finished() const = 0;

  virtual const boost::optional<const std::string> get_error() const = 0;

protected:
  boost::weak_ptr<tcp_client> conn;
};
