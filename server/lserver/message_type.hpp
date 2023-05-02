#pragma once
#include <cstdint>
#include <vector>
const std::vector<uint8_t> END_OF_SENDING_DATA{'\r', '\n'};

typedef enum request_type : uint8_t {
  PING = 1,
  GET_DATA = 2,
  DATA_RECONNECT = 3,
} REQUEST_TYPE;

enum response_type : uint8_t {
  PONG = 1,
  PORT = 2,
  GET_PORT_FAILED = 3,
  SEND_DATA_FAILED = 4,
  INVALID_REQUEST = 5,
  NO_DATA_CHANNEL = 6,
  DATA_CHANNEL_IS_BUSY = 7,
};

request_type request_type_from_byte(uint8_t byte);