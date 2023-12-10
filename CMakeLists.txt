cmake_minimum_required(VERSION 3.0)
project(bakagamon C)

set(CMAKE_C_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories("include" "src")
file(GLOB srcs "src/*.c" "include/*.h")


add_executable(bakagamon ${srcs} src/Input.c src/main.c src/Render.c src/Board.c src/Move.c src/State.c include/History.h src/History.c include/Leaderboard.h src/Leaderboard.c include/Save.h src/Save.c include/ModalList.h src/ModalList.c)
target_link_libraries(bakagamon PRIVATE ${CURSES_LIBRARIES})