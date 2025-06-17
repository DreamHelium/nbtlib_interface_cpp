cmake_minimum_required(VERSION 3.15)

project(nbt_interface_cpp LANGUAGES C CXX)

find_package(PkgConfig)

pkg_search_module(GIO REQUIRED gio-2.0)

set(NIMM_SOURCE nbt_interface.cpp)

set(NIMM_HEADER nbt_interface.hpp)

add_subdirectory(libnbt)

add_library(dhnbt_interface_cpp SHARED ${NIMM_SOURCE} ${NIMM_HEADER})

target_link_libraries(dhnbt_interface_cpp PUBLIC gio-2.0 nbt-glib)
target_include_directories(dhnbt_interface_cpp PUBLIC ${GIO_INCLUDE_DIRS})

add_executable(dhnbt_interface_test test.cpp)
target_link_libraries(dhnbt_interface_test PUBLIC dhnbt_interface_cpp)