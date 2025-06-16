file(GLOB TEST_SOURCES *.cpp)

add_executable(FixEngineTests ${TEST_SOURCES})

target_link_libraries(FixEngineTests
    gtest
    gtest_main
    FixEngineLib
    spdlog
)

include(GoogleTest)
gtest_discover_tests(FixEngineTests)
