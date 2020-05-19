cmake_minimum_required(VERSION 3.15.0)
project(maze_builder VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(maze_builder main.cpp src/maze_gen.cpp lib/cxxopts/cxxopts.hpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
