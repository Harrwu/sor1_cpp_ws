cmake_minimum_required(VERSION 3.10)
project(HFT)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(core_logic 
    src/order.cpp
    src/strategy.cpp
    src/feed_handler.cpp
)

add_executable(data src/data.cpp)
target_link_libraries(data PRIVATE core_logic)