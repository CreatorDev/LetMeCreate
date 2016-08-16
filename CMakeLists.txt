cmake_minimum_required(VERSION 2.8)

project(LetMeCreate)


set(LETMECREATE_MAJOR_VERSION 1)
set(LETMECREATE_MINOR_VERSION 0)
set(LETMECREATE_PATCH_VERSION 1)
set(LETMECREATE_VERSION ${LETMECREATE_MAJOR_VERSION}.${LETMECREATE_MINOR_VERSION}.${LETMECREATE_PATCH_VERSION})
set(PROJECT_VERSION ${LETMECREATE_VERSION})

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

file(GLOB core_srcs src/core/*.c)
file(GLOB click_srcs src/click/*.c)

include_directories(include)

# Create targets
add_library(letmecreate_core SHARED ${core_srcs})
add_library(letmecreate_click SHARED ${click_srcs})
target_link_libraries(letmecreate_click letmecreate_core)

target_compile_definitions(letmecreate_core PUBLIC "LETMECREATE_CORE_DEBUG=$<CONFIG:Debug>")
target_compile_definitions(letmecreate_click PUBLIC "LETMECREATE_CLICK_DEBUG=$<CONFIG:Debug>")


target_include_directories(
    letmecreate_core PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)
target_include_directories(
    letmecreate_click PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)


set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Note: PROJECT_VERSION is used as a VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * targets_export_name
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Targets:
#   * <prefix>/lib/libbar.a
#   * <prefix>/lib/libbaz.a
#   * header location after install: <prefix>/include/foo/Bar.hpp
#   * headers can be included by C++ code `#include <foo/Bar.hpp>`
install(
    TARGETS letmecreate_core letmecreate_click
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

# Headers:
#   * Source/foo/Bar.hpp -> <prefix>/include/foo/Bar.hpp
#   * Source/foo/Baz.hpp -> <prefix>/include/foo/Baz.hpp
install(
    DIRECTORY "include/letmecreate"
    DESTINATION "${include_install_dir}"
    FILES_MATCHING PATTERN "*.h"
)

# Config
#   * <prefix>/lib/cmake/Foo/FooConfig.cmake
#   * <prefix>/lib/cmake/Foo/FooConfigVersion.cmake
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
#   * <prefix>/lib/cmake/Foo/FooTargets.cmake
install(
    EXPORT "${targets_export_name}"
    DESTINATION "${config_install_dir}"
)


if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)
