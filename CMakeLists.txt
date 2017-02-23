cmake_minimum_required(VERSION 2.8)

project(LetMeCreate)


set(LETMECREATE_MAJOR_VERSION 1)
set(LETMECREATE_MINOR_VERSION 5)
set(LETMECREATE_PATCH_VERSION 1)
set(LETMECREATE_VERSION ${LETMECREATE_MAJOR_VERSION}.${LETMECREATE_MINOR_VERSION}.${LETMECREATE_PATCH_VERSION})
set(PROJECT_VERSION ${LETMECREATE_VERSION})

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

file(GLOB bosch_srcs src/bosch/*.c)
file(GLOB core_srcs src/core/*.c)
file(GLOB click_srcs src/click/*.c)
file(GLOB rpisensehat_srcs src/rpisensehat/*.c)

file(GLOB bosch_hdrs include/letmecreate/bosch/*.h)
file(GLOB core_hdrs include/letmecreate/core/*.h)
file(GLOB click_hdrs include/letmecreate/click/*.h)
file(GLOB rpisensehat_hdrs include/letmecreate/rpisensehat/*.h)

include_directories(include)

# Create targets
add_library(letmecreate_core SHARED ${core_srcs} ${core_hdrs})
add_library(letmecreate_click SHARED ${click_srcs} ${click_hdrs})
add_library(letmecreate_bosch SHARED ${bosch_srcs} ${bosch_hdrs})
add_library(letmecreate_rpisensehat SHARED ${rpisensehat_srcs} ${rpisensehat_hdrs})
target_link_libraries(letmecreate_core pthread)
target_link_libraries(letmecreate_click letmecreate_core letmecreate_bosch)
target_link_libraries(letmecreate_rpisensehat letmecreate_core)

# Export symbols
include(GenerateExportHeader)
generate_export_header(letmecreate_bosch EXPORT_FILE_NAME "include/letmecreate/bosch/export.h")
generate_export_header(letmecreate_core EXPORT_FILE_NAME "include/letmecreate/core/export.h")
generate_export_header(letmecreate_click EXPORT_FILE_NAME "include/letmecreate/click/export.h")
generate_export_header(letmecreate_rpisensehat EXPORT_FILE_NAME "include/letmecreate/rpisensehat/export.h")
set_target_properties(letmecreate_bosch PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(letmecreate_core PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(letmecreate_click PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(letmecreate_rpisensehat PROPERTIES C_VISIBILITY_PRESET hidden)

# Enable warnings
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -ggdb3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -Wextra -O2")


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
target_include_directories(
    letmecreate_bosch PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)
target_include_directories(
    letmecreate_rpisensehat PUBLIC
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

write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install(
    TARGETS letmecreate_core letmecreate_click letmecreate_bosch letmecreate_rpisensehat
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

install(
    DIRECTORY "include/letmecreate"
    DESTINATION "${include_install_dir}"
    FILES_MATCHING PATTERN "*.h"
)

install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

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
