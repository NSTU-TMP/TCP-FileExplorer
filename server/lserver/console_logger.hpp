#pragma once

#include "logger.hpp"
class console_logger : public logger {
public:
  void info(std::string ctx, std::string msg) override;
  void warning(std::string ctx, std::string msg) override;
  void error(std::string ctx, std::string msg) override;

private:
  void print(std::string status, std::string ctx, std::string msg);
};
