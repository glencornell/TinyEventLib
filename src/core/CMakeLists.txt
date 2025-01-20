set(SOURCES
  QAbstractEventDispatcher.cpp
  QCoreApplication.cpp
  QEventLoop.cpp
  QObject.cpp
  QSocketNotifier.cpp
  QTimer.cpp
)

if(WIN32)
  list(APPEND SOURCES windows_specific.cpp)
elseif(UNIX AND NOT APPLE)
  list(APPEND SOURCES
    QEventDispatcherUNIX.cpp
    QPosixSignal.cpp
  )
elseif(APPLE)
  list(APPEND SOURCES mac_specific.cpp)
endif()

add_library(QtCoreLite ${SOURCES})
target_include_directories(QtCoreLite
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/QtLite/core>
)

# Set coverage flags:
target_compile_options(QtCoreLite PRIVATE
  $<$<CONFIG:Coverage>:${GLOBAL_COVERAGE_COMPILE_OPTIONS}>
)
target_link_options(QtCoreLite PUBLIC
  $<$<CONFIG:Coverage>:-g>
  -Wl,-u,globalEventDispatcher
)
target_link_libraries(QtCoreLite PUBLIC
  $<$<CONFIG:Coverage>:gcov>
)
