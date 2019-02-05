cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 11)
set(LIBRARY_NAME model)

include_directories(include ../app/include)

add_library(${LIBRARY_NAME} STATIC src/model.cpp src/processesinfo.cpp)

