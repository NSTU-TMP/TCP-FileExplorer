#pragma once

#include <string>

class logger {
public:
  virtual void info(std::string ctx, std::string msg) = 0;
  virtual void warning(std::string ctx, std::string msg) = 0;
  virtual void error(std::string ctx, std::string msg) = 0;
};
