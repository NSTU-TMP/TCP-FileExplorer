#include "console_logger.hpp"
#include <boost/algorithm/string/trim.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>

void console_logger::info(std::string ctx, std::string msg) {
  this->print("INFO", ctx, msg);
}

void console_logger::warning(std::string ctx, std::string msg) {
  this->print("WARNING", ctx, msg);
}

void console_logger::error(std::string ctx, std::string msg) {
  this->print("ERROR", ctx, msg);
}

void console_logger::print(std::string status, std::string ctx,
                           std::string msg) {

  // std::time_t time = std::time({});
  // char time_string[std::size("hh:mm:ss")];
  // std::strftime(std::data(time_string), std::size(time_string), "%FT%TZ",
  //               std::gmtime(&time));

  // auto now = std::chrono::system_clock::now();
  // std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  // std::string now_time_string = std::ctime(&now_time);

  boost::trim(status);
  boost::trim(ctx);
  boost::trim(msg);

  std::string full_msg =
      "[" + status + "]" + "[" + "]" + "[" + ctx + "] " + msg;
  std::cout << full_msg << "\n";
}
