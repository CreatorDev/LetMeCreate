set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/bin)

add_library(common OBJECT common.c)

add_executable(test_switch test_switch.c $<TARGET_OBJECTS:common>)
target_link_libraries(test_switch letmecreate_core)
install(TARGETS test_switch RUNTIME DESTINATION bin)

add_executable(test_adc test_adc.c $<TARGET_OBJECTS:common>)
target_link_libraries(test_adc letmecreate_core)
install(TARGETS test_adc RUNTIME DESTINATION bin)

add_executable(test_uart test_uart.c $<TARGET_OBJECTS:common>)
target_link_libraries(test_uart letmecreate_core)
install(TARGETS test_uart RUNTIME DESTINATION bin)

