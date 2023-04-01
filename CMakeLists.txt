cmake_minimum_required(VERSION 3.0)

project(pisamreader)
set( CMAKE_EXPORT_COMPILE_COMMANDS true)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories(
    ${CMAKE_SOURCE_DIR}/include,
    ${CMAKE_SOURCE_DIR}/proisam++
    )


set(CMAKE_CXX_STANDARD 20)

add_subdirectory(proisam++)

set(SOURCES src/main.cpp)

add_executable(pisamreader  ${SOURCES})

target_link_libraries(pisamreader PUBLIC proisam++)
