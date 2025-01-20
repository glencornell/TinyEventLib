add_executable(test_core
  test_QSignal.cpp
  test_QObject.cpp
  test_QEvent.cpp
  test_QTimer.cpp
  test_QSocketNotifier.cpp
  test_QCoreApplication.cpp
  test_QPosixSignal.cpp
)
target_link_libraries(test_core PUBLIC QtCoreLite gtest_main gtest)

gtest_discover_tests(test_core)
add_dependencies(check test_core)
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  add_dependencies(coverage test_core)
endif()
