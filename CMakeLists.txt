cmake_minimum_required(VERSION 3.5)

project(WebServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
    HEADERS
    external/bimap.hpp include/Socket.hpp include/Utility.hpp include/Server.hpp
    include/common.h
    external/tl/expected.hpp
)

set(
    SOURCES
    src/Socket.cpp src/Utility.cpp
)

add_executable(WebServer main.cpp
    ${SOURCES}
    ${HEADERS}
)

include(GNUInstallDirs)
include_directories(include/ external/)

install(TARGETS WebServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
