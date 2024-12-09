if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  find_program(LCOV lcov REQUIRED)
  find_program(GENHTML genhtml REQUIRED)

  # directory where coverage reports are stored
  set(COVERAGE_DIR "${CMAKE_BINARY_DIR}/coverage")

  # Custom target to run tests and collect coverage data. Although you can run `tests/test_*`, or `ctest`, it is
  # recommended to run the unit tests in a CI automation using the `coverage` target below because this will generate
  # the HTML code coverage report following test execution.  NOTE: you will need to call `add_dependencies(coverage
  # ...)` for any test applications that are needed by the tests.
  add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}
    COMMAND ${LCOV} --capture --initial --directory . --base-directory ${CMAKE_SOURCE_DIR} --output-file ${COVERAGE_DIR}/coverage.info
    COMMAND ${CMAKE_COMMAND} -E env COVERAGE_DIR=${CMAKE_BINARY_DIR}/coverage ${CMAKE_CTEST_COMMAND} --output-on-failure --timeout 20s || ${CMAKE_COMMAND} -E echo "test failed but coverage will proceed"
    COMMAND ${LCOV} --capture --directory . --base-directory ${CMAKE_SOURCE_DIR} --output-file ${COVERAGE_DIR}/coverage.info
    COMMAND ${LCOV} --remove ${COVERAGE_DIR}/coverage.info '/usr/*' '*/gtest/*' '*/tests/*' --output-file ${COVERAGE_DIR}/coverage_filtered.info
    COMMAND ${GENHTML} ${COVERAGE_DIR}/coverage_filtered.info --output-directory ${COVERAGE_DIR}/html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running unit tests and generating coverage report..."
  )

  add_custom_command(TARGET coverage POST_BUILD
    COMMAND /bin/true
    COMMAND ${CMAKE_COMMAND} -E echo "Coverage Report: file://${COVERAGE_DIR}/html/index.html"
    )
endif()
