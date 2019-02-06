cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(include ../view/include ../model/include)

add_executable(${CMAKE_PROJECT_NAME} src/ctop.cpp)
add_dependencies(${CMAKE_PROJECT_NAME} model)
add_dependencies(${CMAKE_PROJECT_NAME} view)
target_link_libraries(${CMAKE_PROJECT_NAME} -L../model -lmodel)
target_link_libraries(${CMAKE_PROJECT_NAME} -L../view -lview)
target_link_libraries(${CMAKE_PROJECT_NAME} -lpthread)
qt5_use_modules(${CMAKE_PROJECT_NAME} Widgets Core Gui)

