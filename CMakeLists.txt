cmake_minimum_required(VERSION 2.8)
project(CMD_LINE_PARSER)

enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)