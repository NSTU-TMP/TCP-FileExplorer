#include "messages_types.hpp"

info_request_type byte_into_info_request(uint8_t byte) {
  switch (byte) {
  case '1':
    return info_request_type::PING;
  case '2':
    return info_request_type::GET_FS_ENTRY;
  case '3':
    return info_request_type::RECONNECT_DATA_CHANNEL;
  default:
    throw invalid_info_request();
  }
}

std::string info_request_to_string(info_request_type request_type) {
  switch (request_type) {
  case info_request_type::PING:
    return "PING";
  case info_request_type::RECONNECT_DATA_CHANNEL:
    return "RECONNECT_DATA_CHANNEL";
  case info_request_type::GET_FS_ENTRY:
    return "GET_FS_ENTRY";
  }
}

uint8_t info_response_into_byte(info_response_type response) {
  switch (response) {
  case info_response_type::OK:
    return '1';
  case info_response_type::ERROR:
    return '2';
  }
}

uint8_t fs_data_response_into_byte(fs_data_response_type response) {
  switch (response) {
  case fs_data_response_type::FILE:
    return '1';
  case fs_data_response_type::DIRECTORY:
    return '2';
  }
}
