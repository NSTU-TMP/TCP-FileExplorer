#include "message_type.hpp"

request_type request_type_from_byte(uint8_t byte) {
  switch (byte) {
    case 1:
      return PING;
      break;
    case 2:
      return GET_DATA;
      break;
    case 3:
      return DATA_RECONNECT;
      break;

    default:
      return PING;
      break;
  }
}
