cmake_minimum_required(VERSION 3.25)
project(bakagamon C)

set(CMAKE_C_STANDARD 17)

find_package(curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories("include" "src")
file(GLOB srcs "src/*.c" "include/*.h")

add_executable(bakagamon ${srcs})
target_link_libraries(bakagamon ${CURSES_LIBRARIES})
