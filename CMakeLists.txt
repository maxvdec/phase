cmake_minimum_required(VERSION 3.15)
project(Phase LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRCS ./phase/*.cpp)

find_package(Curses REQUIRED)

add_executable(phase ${SRCS})

target_link_libraries(phase ${CURSES_LIBRARIES})

target_include_directories(phase PUBLIC include ${CURSES_INCLUDE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_PATH ${CMAKE_SOURCE_DIR})

add_definitions(-DCMAKE_PATH="${CMAKE_PATH}")

