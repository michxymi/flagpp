add_executable(basic_usage basic_usage.cpp)
target_link_libraries(basic_usage PRIVATE flagplusplus::flagplusplus)

add_executable(thread_safety thread_safety.cpp)
target_link_libraries(thread_safety PRIVATE 
    flagplusplus::flagplusplus
    Threads::Threads
)

set_target_properties(basic_usage thread_safety
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

option(FLAGPP_INSTALL_EXAMPLES "Install FlagPlusPlus examples" OFF)
if(FLAGPP_INSTALL_EXAMPLES)
    install(TARGETS basic_usage thread_safety
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/examples)
    
    install(FILES 
            ${CMAKE_CURRENT_SOURCE_DIR}/basic_usage.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/thread_safety.cpp
            DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples)
endif()
