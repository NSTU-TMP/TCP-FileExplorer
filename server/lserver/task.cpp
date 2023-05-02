#include "task.hpp"

task::task(boost::weak_ptr<tcp_client> _conn) : conn(_conn) {}
