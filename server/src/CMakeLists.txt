cmake_minimum_required(VERSION 3.10)

project(ServerLibraryTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(Executable.out main.cpp)

target_link_libraries(Executable.out ${Boost_LIBRARIES})

target_include_directories(
    Executable.out
    PRIVATE
    ${SMAKE_CURRENT_SOURCE_DIR}/../lib
)

target_link_directories(
    Executable.out   
    PRIVATE
    ${SMAKE_CURRENT_SOURCE_DIR}/../lib
)

target_link_libraries(Executable.out lib)
