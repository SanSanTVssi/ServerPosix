cmake_minimum_required(VERSION 3.21)
project(Server)

set(CMAKE_CXX_STANDARD 23)

add_executable(Server main.cpp server.cpp server.h logging.cpp logging.h old.h base.h base.cpp forward_list.cpp forward_list.h IServer.h)
