cmake_minimum_required(VERSION 3.10)

project(ServerLibrary LANGUAGES CXX)

add_library(lib STATIC 
  handler.cpp
  handler.hpp
  ip_addr.cpp
  ip_addr.hpp
  server.cpp
  server.hpp
  tcp_client.hpp
  tcp_listener.cpp
  tcp_listener.hpp
  message_type.h
)
