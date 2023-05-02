cmake_minimum_required(VERSION 3.10)

project(ServerLibrary LANGUAGES CXX)


find_package(Boost REQUIRED COMPONENTS system filesystem thread)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})


add_library(lserver STATIC 
  task.cpp
  task.hpp
  fs_task.cpp
  fs_task.hpp
  handler.cpp
  handler.hpp
  info_client.cpp
  info_client.hpp
  server.cpp
  server.hpp
  message_type.cpp
  message_type.hpp
  )

target_link_libraries(lserver ${Boost_LIBRARIES})

target_include_directories(
    lserver
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../lnet
)

target_link_directories(
    lserver   
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../lnet
)
target_link_libraries(lserver lnet)
