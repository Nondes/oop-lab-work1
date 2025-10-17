cmake_minimum_required(VERSION 3.14)
project(lab1_complex CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RUN_TESTS "Run assert-based tests" ON)

add_library(complex_lib
    scr/complex.cpp
)
target_include_directories(complex_lib PUBLIC include)

add_executable(app
    scr/main.cpp
)
target_link_libraries(app PRIVATE complex_lib)

if(RUN_TESTS)
    target_compile_definitions(app PRIVATE RUN_TESTS)
endif()
