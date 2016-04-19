cmake_minimum_required(VERSION 2.8)

project(LetMeCreate)

option(BUILD_EXAMPLES "Build examples" OFF)

set(letmecreate_VERSION_MAJOR 0)
set(letmecreate_VERSION_MINOR 1)

set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} -Wall -ggdb3")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples/bin)

include_directories(include)
file(GLOB core_srcs src/core/*.c)
file(GLOB click_srcs src/click/*.c)

add_library(letmecreate_core SHARED ${core_srcs})
add_library(letmecreate_click SHARED ${click_srcs})

install(TARGETS letmecreate_core LIBRARY DESTINATION lib)
install(TARGETS letmecreate_click LIBRARY DESTINATION lib)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)
