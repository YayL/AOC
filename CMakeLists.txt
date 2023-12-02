cmake_minimum_required(VERSION 3.23)

project(AOC VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-Wall")

add_subdirectory("submodule/String-Formatter")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/includes/src/*.c
                                            ${CMAKE_CURRENT_SOURCE_DIR}/2023/day02/part2.c)

message(STATUS "SOURCES: ${SOURCES}")

add_executable(AOC ${SOURCES})

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

target_include_directories(AOC PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/include
        ${CMAKE_CURRENT_SOURCE_DIR}/submodule/String-Formatter/include)

# Link libraries
target_link_libraries(AOC PRIVATE fmt)
