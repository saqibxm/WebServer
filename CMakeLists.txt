cmake_minimum_required(VERSION 3.5)

project(WebServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
    HEADERS
    common.h
    external/bimap.hpp external/tl/expected.hpp
    include/Socket.hpp
    include/Utility.hpp
    include/Server.hpp
    include/Request.hpp
    include/Parser.hpp
)

set(
    SOURCES
    src/Socket.cpp
    src/Utility.cpp
    src/Server.cpp
    src/Request.cpp
    src/Parser.cpp
)

add_executable(WebServer main.cpp
    ${SOURCES}
    ${HEADERS}
)

include(GNUInstallDirs)
include_directories(./ include/ external/)

install(TARGETS WebServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
