#pragma once

#ifndef COMMON_H
#define COMMON_H

#include <filesystem>
#include <string>
#include <iosfwd>
#include <chrono>

#if defined(_WIN32) || defined(_MSC_VER) || defined(__MINGW64__)
#define PLATFORM_WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#define PLATFORM_POSIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#define CR '\r'
#define LF '\n'
#define CRLF "\r\n"

namespace fs = std::filesystem;
namespace crn = std::chrono;

#endif // COMMON_H
