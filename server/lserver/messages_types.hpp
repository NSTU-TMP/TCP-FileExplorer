#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

const std::vector<uint8_t> END_OF_SENDING_DATA{'\r', '\n'};

enum class info_request_type {
  PING,
  GET_FS_ENTRY,
  RECONNECT_DATA_CHANNEL,
};

enum class info_response_type {
  OK,
  ERROR,
};

enum class fs_data_response_type {
  FILE,
  DIRECTORY,
};

class invalid_info_request : std::exception {};

info_request_type byte_into_info_request(uint8_t byte);
std::string info_request_to_string(info_request_type request_type);

uint8_t info_response_into_byte(info_response_type response);

uint8_t fs_data_response_into_byte(fs_data_response_type response);
