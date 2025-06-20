cmake_minimum_required(VERSION 3.15)
project(FixServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB FIX_SOURCES src/fix/*.cpp src/net/*.cpp)
file(GLOB FIX_HEADERS include/fix/*.h include/net/*.h)

# -------------------------- External Libs ------------------------------------
include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.13.0 # or latest stable
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
# -----------------------------------------------------------------------------

# Make library
add_library(FixEngineLib STATIC ${FIX_SOURCES} ${FIX_HEADERS})
target_include_directories(FixEngineLib PUBLIC include)
target_link_libraries(FixEngineLib PUBLIC spdlog::spdlog)

# Server executable
add_executable(fix_server.out src/mainServer.cpp ${FIX_SOURCES})
target_link_libraries(fix_server.out PRIVATE spdlog::spdlog)

# Client executable
add_executable(fix_client.out src/mainClient.cpp ${FIX_SOURCES})
target_link_libraries(fix_client.out PRIVATE spdlog::spdlog)


# Testing
enable_testing()
add_subdirectory(test)
