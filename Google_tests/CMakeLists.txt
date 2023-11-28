project(Google_tests)
add_subdirectory(lib)
set(CMAKE_CXX_STANDARD 17)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run BoardTest.cpp MoveTest.cpp)
target_link_libraries(Google_Tests_run PUBLIC gtest gtest_main bakagamon_lib)

add_test(bakagamon-tests Google_Tests_run)