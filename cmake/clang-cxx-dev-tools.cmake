file(GLOB_RECURSE
     ALL_CXX_SOURCE_FILES
     src/fix/*.cpp src/net/*.cpp
    )

# clang-format
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  add_custom_target(
    clang-format
    COMMAND /usr/bin/clang-format
    -style=Google
    --dry-run
    ${ALL_CXX_SOURCE_FILES}
    )
endif()
