cmake_minimum_required(VERSION 3.14)
project(FlagPlusPlus 
        VERSION 0.1.0
        DESCRIPTION "A modern, header-only C++ feature flag library"
        HOMEPAGE_URL "https://github.com/yourusername/flagplusplus"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(flagplusplus INTERFACE)
add_library(flagplusplus::flagplusplus ALIAS flagplusplus)

target_include_directories(flagplusplus
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(flagplusplus INTERFACE cxx_std_17)

install(TARGETS flagplusplus
    EXPORT flagplusplus-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES include/flagpp.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/flagplusplus-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/flagplusplus-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/flagplusplus
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/flagplusplus-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/flagplusplus-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/flagplusplus-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/flagplusplus
)

install(EXPORT flagplusplus-targets
    FILE flagplusplus-targets.cmake
    NAMESPACE flagplusplus::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/flagplusplus
)

option(FLAGPP_BUILD_EXAMPLES "Build FlagPlusPlus examples" ON)
option(FLAGPP_BUILD_TESTS "Build FlagPlusPlus tests" ON)

find_package(Threads REQUIRED)

if(FLAGPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(FLAGPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

export(EXPORT flagplusplus-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/flagplusplus-targets.cmake"
       NAMESPACE flagplusplus::)

export(PACKAGE flagplusplus)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/flagplusplus.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/flagplusplus.pc
    @ONLY
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/flagplusplus.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
