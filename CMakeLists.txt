cmake_minimum_required(VERSION 3.14)
project(lab3_stack CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(RUN_TESTS "Run assert-based tests" ON)
option(RUN_CLI   "Run simple console interface" OFF)
add_library(core
  src/complex.cpp
  src/quaternion.cpp
  src/stackcalc.cpp
)
target_include_directories(core PUBLIC include)
add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE core)
if(RUN_TESTS)
  target_compile_definitions(app PRIVATE RUN_TESTS)
endif()
if(RUN_CLI)
  target_compile_definitions(app PRIVATE RUN_CLI)
endif()
