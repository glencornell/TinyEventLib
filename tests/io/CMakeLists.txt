add_executable(test_io
  test_QIODevice.cpp
)
target_link_libraries(test_io PUBLIC QtIOLite gtest_main gtest)

gtest_discover_tests(test_io)
add_dependencies(check test_io)
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  add_dependencies(coverage test_io)
endif()
