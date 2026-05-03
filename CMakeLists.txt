cmake_minimum_required(VERSION 3.10)
project(CubeEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_CLIENT "Build the Cube client" ON)
option(BUILD_SERVER "Build the Cube standalone server" ON)

add_subdirectory(enet)
add_subdirectory(src)