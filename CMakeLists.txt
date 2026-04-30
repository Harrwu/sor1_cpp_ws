cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories: tell CMake where to find your headers
include_directories(src/RISC-V_Emulator)

# Create a library for core logic to share between main and test
add_library(core_logic 
    src/RISC-V_Emulator/cpu.cpp 
    src/RISC-V_Emulator/memory.cpp
)

# Build the main executable
add_executable(app src/RISC-V_Emulator/main.cpp)
target_link_libraries(app PRIVATE core_logic)
