cmake_minimum_required(VERSION 3.10)

project(NetLibrary LANGUAGES CXX)

add_library(lnet STATIC 
  tcp_client.hpp
  tcp_client.cpp
  tcp_listener.cpp
  tcp_listener.hpp
  ip_addr.cpp
  ip_addr.hpp
)
