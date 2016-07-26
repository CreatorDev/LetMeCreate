cmake_minimum_required(VERSION 2.8.8)

project(LetMeCreate)

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/letmecreate)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

set(LETMECREATE_MAJOR_VERSION 1)
set(LETMECREATE_MINOR_VERSION 0)
set(LETMECREATE_PATCH_VERSION 1)

set(LETMECREATE_VERSION ${LETMECREATE_MAJOR_VERSION}.${LETMECREATE_MINOR_VERSION}.${LETMECREATE_PATCH_VERSION})

set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} -Wall -ggdb3")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(include/letmecreate)

file(GLOB core_srcs src/core/*.c)
file(GLOB click_srcs src/click/*.c)

add_library(letmecreate_core SHARED ${core_srcs})
add_library(letmecreate_click SHARED ${click_srcs})

install(TARGETS letmecreate_core LIBRARY DESTINATION lib)
install(TARGETS letmecreate_click LIBRARY DESTINATION lib)
install(DIRECTORY include DESTINATION .)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)

file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
configure_file(LetMeCreateConfig.cmake.in
  "${PROJECT_BINARY_DIR}/LetMeCreateConfig.cmake" @ONLY)

configure_file(LetMeCreateConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/LetMeCreateConfigVersion.cmake" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}/LetMeCreateConfig.cmake"
  "${PROJECT_BINARY_DIR}/LetMeCreateConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

export(PACKAGE LetMeCreate)
