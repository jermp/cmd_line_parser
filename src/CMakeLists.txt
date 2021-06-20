cmake_minimum_required(VERSION 2.8)
project(CMD_PARSER)
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif ()
MESSAGE(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif ()
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-missing-braces")
    if (USE_SANITIZERS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    endif ()

endif ()

enable_testing()

add_executable(example ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp)
add_executable(test_parse ${CMAKE_CURRENT_SOURCE_DIR}/test_parse.cpp)

add_test(test_parse_CTEST ${CMAKE_BINARY_DIR}/test_parse)
