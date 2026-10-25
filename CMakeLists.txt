# /CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(excalibur C)

add_executable(excalibur
  src/parser.c
  src/forward_sim.c

  main.c
)

target_include_directories(excalibur PRIVATE include)
target_compile_options(excalibur PRIVATE -Ofast -march=native -flto)

