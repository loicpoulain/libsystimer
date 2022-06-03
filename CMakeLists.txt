cmake_minimum_required (VERSION 2.6)
project (libsystimer)

# systimer library (C)
add_library(systimer lib/core.c)
target_include_directories(systimer PUBLIC include)

# QSysTimer library (QT/c++)
find_package(Qt5 5.6 CONFIG REQUIRED
  Core
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(QSysTimer lib/QSysTimer.cpp include/QSysTimer.h)
target_include_directories(QSysTimer PUBLIC include)
target_link_libraries(QSysTimer Qt5::Core systimer)

# Tools...
add_executable(systimertest tools/systimertest.c)
target_include_directories(systimertest PRIVATE include)
target_link_libraries(systimertest PRIVATE systimer)

