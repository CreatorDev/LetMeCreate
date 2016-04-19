cmake_minimum_required(VERSION 2.8)

project(LetMeCreate)

set(letmecreate_VERSION_MAJOR 0)
set(letmecreate_VERSION_MINOR 1)

set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} -Wall -ggdb3")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(include)
file(GLOB core_srcs src/core/*.c)

add_library(letmecreate_core SHARED ${core_srcs})

install(TARGETS letmecreate_core LIBRARY DESTINATION lib)
