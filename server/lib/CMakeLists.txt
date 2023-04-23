cmake_minimum_required(VERSION 3.10)

project(ServerLibrary LANGUAGES CXX)


add_library(lib STATIC 
    server.cpp 
    handler.cpp
)
