cmake_minimum_required(VERSION 3.10)
project(RISC_V EMULATOR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.3 REQUIRED)

include_directories(include)

# Create a library for core logic to share between main and test
add_library(core_logic 
    src/cpu.cpp 
    src/memory.cpp
)

# Build the main executable
add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE core_logic)

add_executable(graphics src/graphics/graphics.cpp src/graphics/glad.c)
target_link_libraries(graphics GL dl glfw)