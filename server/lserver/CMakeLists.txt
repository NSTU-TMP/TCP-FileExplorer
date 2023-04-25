cmake_minimum_required(VERSION 3.10)

project(ServerLibrary LANGUAGES CXX)

add_library(lserver STATIC 
  handler.cpp
  handler.hpp
  server.cpp
  server.hpp
  message_type.cpp
  message_type.hpp
)

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



# target_include_directories(
#   lserver 
#   PUBLIC 
#   ${SMAKE_CURRENT_SOURCE_DIR}../lnet
#   )

# add_subdirectory(${SMAKE_CURRENT_SOURCE_DIR}../lnet)
# target_link_libraries(lserver lnet)
