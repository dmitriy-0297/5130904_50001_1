cmake_minimum_required(VERSION 3.10)
project(Lab14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab 
    src/main.cpp
    src/Geometry.cpp
    src/Commands.cpp
)

target_include_directories(lab PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
