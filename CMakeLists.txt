# /CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(excalibur C)

add_executable(excalibur
  src/parser.c
  src/deciders/forward_sim.c
  src/deciders/backward.c
  main.c
)

target_include_directories(excalibur PRIVATE include)
target_compile_options(excalibur PRIVATE -O3 -march=native -flto)