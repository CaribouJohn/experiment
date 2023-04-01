set(SOURCES proisam.cpp)
set(HEADERS proisam.hpp)
add_library(proisam++ ${SOURCES} ${HEADERS})
target_include_directories(proisam++ PRIVATE ${CMAKE_SOURCE_DIR}/proisam++)
