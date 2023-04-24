#pragma once
#include <cstdint>
#include <vector>
const std::vector<uint8_t> END_OF_SENDING_DATA{'\r', '\n'};
typedef enum request_type {
  PING = 1,
  GET_DATA = 2,
  GET_PORT = 3,
} REQUEST_TYPE;

request_type request_type_from_byte(uint8_t byte);
