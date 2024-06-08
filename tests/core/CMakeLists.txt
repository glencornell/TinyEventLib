add_executable(test_core
  test_function_ref.cpp
  test_QSignal.cpp
  test_QObject.cpp
)
target_link_libraries(test_core PUBLIC QtCoreLite gtest_main gtest)

gtest_discover_tests(test_core)
add_dependencies(check test_core)
