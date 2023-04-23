#ifndef LIB_MESSAGE_TYPE_H_
#define LIB_MESSAGE_TYPE_H_

typedef enum request_type {
  PING = 0,
  EXIT = 1,
  GET_DIR_DATA = 2,
  GET_FILE_DATA = 3,
} REQUEST_TYPE;

#endif