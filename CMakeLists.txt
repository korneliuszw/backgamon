cmake_minimum_required(VERSION 3.25)
project(bakagamon C)

set(CMAKE_C_STANDARD 17)

find_package(curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories("include" "src")
file(GLOB srcs "src/*.c" "include/*.h")


add_executable(bakagamon ${srcs} src/Input.c src/main.c src/Render.c src/Board.c src/Move.c src/State.c include/History.h src/History.c include/Leaderboard.h src/Leaderboard.c include/Save.h src/Save.c include/ModalList.h src/ModalList.c)
target_link_libraries(bakagamon PRIVATE ${CURSES_LIBRARIES})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    add_library(bakagamon_lib STATIC src/Board.c src/Move.c src/State.c src/History.c)
    target_include_directories(bakagamon_lib PUBLIC include)
    add_subdirectory(Google_tests)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
