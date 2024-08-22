cmake_minimum_required(VERSION 3.5)

project(WebServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/server.cpp src/Socket.cpp)
set(HEADERS include/server.hpp include/Socket.hpp)

add_executable(WebServer main.cpp ${SOURCES}
    ${HEADERS})

include(GNUInstallDirs)
include_directories(include/)

install(TARGETS WebServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
