add_executable(test_io
  test_QIODevice.cpp
)
target_link_libraries(test_io PUBLIC QtIOLite QtCoreLite gtest_main gtest)

gtest_discover_tests(test_io)
add_dependencies(check test_io)
