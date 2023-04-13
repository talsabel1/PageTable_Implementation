cmake_minimum_required(VERSION 3.17)
project(Hwk1 C)

set(CMAKE_C_STANDARD 11)

# Add your source files and header files to the project
add_executable(PT os.c pt.c os.h)

# Set compiler flags
target_compile_options(PT PRIVATE -O3 -Wall)