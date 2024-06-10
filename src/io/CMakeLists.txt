add_library(QtIOLite
  QIODevice.cpp
)
target_include_directories(QtIOLite
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/QtLite/io>
)

# Set compiler flags:
target_compile_options(QtIOLite PRIVATE
  $<$<CONFIG:Coverage>:${GLOBAL_COVERAGE_COMPILE_OPTIONS}>
)
target_link_options(QtIOLite PUBLIC
  $<$<CONFIG:Coverage>:-g>
)
target_link_libraries(QtIOLite PUBLIC
  QtCoreLite
  $<$<CONFIG:Coverage>:gcov>
)
