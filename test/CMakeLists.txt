enable_testing()

add_executable(test_fix_engine
  FixParserTest.cpp
  FixEncoderTest.cpp
  FixMessageTest.cpp
)

target_link_libraries(test_fix_engine
  FixEngineLib
  GTest::gtest_main
  GTest::gmock
  spdlog::spdlog
)

include(GoogleTest)
gtest_discover_tests(test_fix_engine)
