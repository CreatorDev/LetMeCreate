set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/bin)

add_library(common OBJECT common.c)

add_executable(test_switch test_switch.c $<TARGET_OBJECTS:common>)
target_link_libraries(test_switch letmecreate_click letmecreate_core)
install(TARGETS test_switch RUNTIME DESTINATION bin)

