add_executable(tests
    ../src/tapes.cpp
    ../src/converter.cpp
    ../src/delimiters.cpp
    test_sorting.cpp
)

target_link_libraries(tests
  GTest::GTest
  GTest::Main
)
