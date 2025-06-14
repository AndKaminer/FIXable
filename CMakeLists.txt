cmake_minimum_required(VERSION 3.15)
project(FixServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB FIX_SOURCES src/fix/*.cpp src/net/*.cpp)

add_executable(fix_server src/main.cpp ${FIX_SOURCES})

#add_subdirectory(third_party/spdlog)

#enable_testing()
#add_subdirectory(test)
