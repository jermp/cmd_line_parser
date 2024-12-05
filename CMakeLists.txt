cmake_minimum_required(VERSION 3.20)
project(CMD_LINE_PARSER)

add_library(CMD_LINE_PARSER INTERFACE)
target_include_directories(CMD_LINE_PARSER
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    enable_testing()
    add_subdirectory(src)
endif()