add_library(QtCoreLite
  QAbstractEventDispatcher.cpp
  QCoreApplication.cpp
  QEventDispatcherUNIX.cpp
  QEventLoop.cpp
  QObject.cpp
  QSocketNotifier.cpp
  QTimer.cpp
)
target_include_directories(QtCoreLite
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/QtLite/core>
)

# Set coverage flags:
target_compile_options(QtCoreLite PUBLIC
  $<$<CONFIG:Coverage>:-O0 --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -g>
)
target_link_options(QtCoreLite PUBLIC
  $<$<CONFIG:Coverage>:-g>
)
target_link_libraries(QtCoreLite PUBLIC
  $<$<CONFIG:Coverage>:gcov>
)
