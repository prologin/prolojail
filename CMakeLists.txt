cmake_minimum_required(VERSION 2.8)
project(prolojail)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++0x -Wall -Wextra -Werror)
endif()

add_executable(
  prolojail
  src/main.cc
  src/run.cc
  src/run.hh
  src/option_parser.cc
  src/option_parser.hh
)

enable_testing()
