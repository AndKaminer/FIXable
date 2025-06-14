cmake_minimum_required(VERSION 3.15)
project(FixServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB FIX_SOURCES src/fix/*.cpp src/net/*.cpp)

include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.13.0 # or latest stable
)

FetchContent_MakeAvailable(spdlog)

add_executable(fix_server.out src/main.cpp ${FIX_SOURCES})
target_link_libraries(fix_server.out PRIVATE spdlog::spdlog)

#enable_testing()
#add_subdirectory(test)
