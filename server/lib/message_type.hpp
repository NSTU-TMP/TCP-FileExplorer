#pragma once
#include <cstdint>
#include <vector>
const std::vector<uint8_t> END_OF_SENDING_DATA{'\r', '\n'};
typedef enum request_type {
  PING = 1,
  GET_DATA = 2,
  GET_PORT = 3,
} REQUEST_TYPE;

enum class response_type {
  PING = 1,
  PORT = 2,
  GET_PORT_FAILED = 3,
  SEND_DATA_FAILED = 4,
  INVALID_REQUEST = 5,
};

request_type request_type_from_byte(uint8_t byte);
