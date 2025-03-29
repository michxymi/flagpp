add_executable(test_flagpp test_flagpp.cpp)
target_include_directories(test_flagpp PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(test_flagpp PRIVATE Threads::Threads)

# Set output directory for tests
set_target_properties(test_flagpp
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add test to CTest
add_test(
    NAME flagpp_tests
    COMMAND test_flagpp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
