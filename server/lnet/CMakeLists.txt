cmake_minimum_required(VERSION 3.10)

project(NetLibrary LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS system filesystem thread)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_library(lnet STATIC 
  tcp_client.hpp
  tcp_client.cpp
  tcp_listener.cpp
  tcp_listener.hpp
  ip_addr.cpp
  ip_addr.hpp
)

target_link_libraries(lnet ${Boost_LIBRARIES})
