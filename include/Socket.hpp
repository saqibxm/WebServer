#pragma once

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <string_view>
#include <optional>

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

using Descriptor = int;
using SocketAddress = struct sockaddr_in;

class Connection
{
    friend class Socket;
    static constexpr Descriptor Invalid = -1, InvalidDescriptor = Invalid;
    static constexpr unsigned MaxReceptionSize = 1024;
public:
    Connection();
    ~Connection();

    bool open(Descriptor, const SocketAddress&);
    void close();
    bool send(const std::string&);
    std::string recv();
    const std::string& get_ipaddr() const;

    explicit operator bool() const { return valid; }

private:
    explicit Connection(Descriptor, const SocketAddress&);
    Descriptor connection = InvalidDescriptor;
    // struct sockaddr_in client;
    std::string ip;
    bool valid;
};

class Socket
{
    inline static Connection InvalidConnection = Connection(Connection::InvalidDescriptor, sockaddr_in());
    static constexpr unsigned short DefaultPort = 1337;
    static constexpr int SOCKADDR_SZ = sizeof(struct sockaddr);
    static constexpr int MAX_CONNECTION = 5;
public:
    // Socket();
    Socket(unsigned short = DefaultPort);
    Socket(std::string_view ip, unsigned short port = DefaultPort);
    ~Socket();

    void Initialize();
    void Shutdown();

    bool Send(const std::string&);
    std::string Receive();

    Connection Listen();
    // Connection Accept();
    std::optional<Connection> ListenNonBlock();

    std::string IpAddr();

private:
    Descriptor file_descriptor;
    int reuse = 1;
    unsigned short port;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    bool Setup();
    void Cleanup();
};


#endif // SOCKET_HPP
