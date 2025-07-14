cmake_minimum_required(VERSION 3.15)

project(nbt_interface_cpp LANGUAGES C CXX
        VERSION 0.1.0
)

include(GNUInstallDirs)

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

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/library_config/dhnbt_interface_cpp.pc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/dhnbt_interface_cpp.pc" @ONLY)

install(FILES "nbt_interface.hpp" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
install(FILES "libnbt/nbt.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/libnbt")

install(TARGETS dhnbt_interface_cpp nbt-glib
        EXPORT dhnbt_interface_cpp
        ARCHIVE DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}"
)