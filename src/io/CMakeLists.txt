add_library(QtIOLite
  QIODevice.cpp
)
target_include_directories(QtIOLite
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/QtLite/core>
)

# Set coverage flags:
target_compile_options(QtIOLite PUBLIC
  $<$<CONFIG:Coverage>:-O0 --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -g>
)
target_link_options(QtIOLite PUBLIC
  $<$<CONFIG:Coverage>:-g>
)
target_link_libraries(QtIOLite PUBLIC
  $<$<CONFIG:Coverage>:gcov>
)
