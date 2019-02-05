cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 11)
set(LIBRARY_NAME view)

set(CMAKE_INCLUDE_CURRENT_DIR ON) 	
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
qt5_wrap_ui(FORM_H forms/view.ui)
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

include_directories(include ../app/include)

add_library(${LIBRARY_NAME} STATIC include/view.h src/view.cpp ${FORM_H})
qt5_use_modules(${LIBRARY_NAME} Widgets Core Gui)

