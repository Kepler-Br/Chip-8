cmake_minimum_required (VERSION 2.6)
project (chip8Emu)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_BINARY_DIR, "build")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

if (UNIX)
     set (CMAKE_CXX_FLAGS "-g -Wall -O3")
endif () 

include_directories (src)
file(GLOB SOURCES "./*.cpp")

add_executable(chip8Emu ${SOURCES})
