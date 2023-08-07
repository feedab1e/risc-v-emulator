cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)

project(rv32i-ima)

add_definitions(-DDEBUG)

file(COPY test DESTINATION ${CMAKE_BINARY_DIR})

add_executable(rv32i-ima main.cpp)
target_compile_features(rv32i-ima PUBLIC cxx_std_23)

install(TARGETS rv32i-ima RUNTIME DESTINATION bin)
