cmake_minimum_required(VERSION 2.8)
project(prolojail)

set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++0x -Wall -Wextra -Werror)
endif()

add_executable(
  prolojail
  src/jail.cc
  src/jail.hh
  src/main.cc
  src/option-parser.cc
  src/option-parser.hh
  src/exec-exception.cc
  src/exec-exception.hh
  src/syscall-manager.cc
  src/syscall-manager.hh
  src/const.hh
)
target_link_libraries(prolojail pthread)

enable_testing()
