#include "Socket.hpp"

#include <iostream>
#include <cassert>

/*
    struct SocketAddress
    {
        SocketAddress(struct sockaddr_in addr) : address(addr) {}
        decltype(address_len)* get_psize() const { return &address_len; }
        struct sockaddr_in get_address() const { return address; }

        struct sockaddr_in address;
        int address_len = sizeof(address);
    } client;
*/

Socket::Socket(unsigned short portnum) : file_descriptor(-1), port(portnum)
{
    Initialize();

    file_descriptor = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    assert(file_descriptor >= 0);

    auto ret = setsockopt(file_descriptor, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if(ret < 0) {
        valid = false;
        std::cerr << "Socket Intitialization Failed\n";
    }

    auto success = Setup();
    valid = success;
    if(!success) std::cerr << "Socket Setup Failed: Unable to Bind!\n";
}

Socket::~Socket()
{
    Cleanup();
    Shutdown();
}

void Socket::Initialize()
{
#ifdef PLATFORM_WIN32
    WSADATA info;
    WORD version = MAKEWORD(2, 2);

    int res = WSAStartup(version, &info);
    if(res != 0) std::cerr << "Error Intializing WinSocks : " << res;
#endif
}

void Socket::Shutdown()
{
#ifdef PLATFORM_WIN32
    WSACleanup();
#endif
}

bool Socket::Setup()
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = ::htons(port);

    auto ret = ::bind(file_descriptor, (const sockaddr*)&server_addr, sizeof(server_addr));

    if(ret != 0) {
        valid = false;
        std::cerr << "Server cannot be started at port " << port << '\n';
    }
    else
        std::cout << "Server Started at Port: " << port << std::endl;

    return ret == 0;
}

void Socket::Cleanup()
{
    ::close(file_descriptor);
}

Connection Socket::Listen()
{
    if(::listen(file_descriptor, Socket::MAX_CONNECTION))
    {
        std::cerr << "Listen Failed!\n";
        return InvalidConnection;
    }

    // return Connection(::accept(file_descriptor, (sockaddr*)&client_addr, Socket::SOCKADDR_SZ));

    auto client_addr_size = sizeof(client_addr);
    auto connection_fd = ::accept(file_descriptor, (sockaddr*)&client_addr, (socklen_t*) &client_addr_size);

    return Connection(connection_fd, client_addr);
}

bool Socket::Send(const std::string &)
{
    // Not Implemented
    return false;
}

std::string Socket::Receive()
{
    // Not Implemented
    return "Not Implemented";
}

bool Socket::Opened() const
{
    return valid;
}

std::string Socket::IpAddr() const
{
    if(server_addr.sin_addr.s_addr == INADDR_ANY) return "LOCALHOST";

    char ip[16] = {};
    ::inet_ntop(server_addr.sin_family, &server_addr.sin_addr, ip, (socklen_t) std::size(ip));
    return ip;
}

bool Socket::Switch(unsigned short newport)
{
    port = newport;
    auto success = Setup();

    return valid = success;
}

Socket::CommonPortType Socket::Port() const
{
    return port;
}

/* Connection */

Connection::Connection() : Connection(InvalidDescriptor, SocketAddress{})
{
    // ...
}

Connection::~Connection()
{
    ::close(connection);
}

// Connection::Connection(Descriptor fd, const SocketAddress &addr) : connection(fd), client(std::move(addr)) /*, valid(connection != Invalid)*/
// {
//     // without std::moving addr, strange stuff happens (its registered with the fd)
//     valid = connection != InvalidDescriptor;
// }

Connection::Connection(Descriptor fd, const SocketAddress &addr) : connection(fd), ip(::inet_ntoa(addr.sin_addr))
{
    valid = connection != InvalidDescriptor;
}


bool Connection::open(Descriptor fd, const SocketAddress &addr)
{
    // client = addr;
    ip = ::inet_ntoa(addr.sin_addr);
    connection = fd;
    return valid = fd != Invalid;
}

bool Connection::send(const std::string &data)
{
    if(!valid) throw std::logic_error("Send on Invalid Connection");
    ssize_t dummy = 0;
    return (dummy = ::send(connection, data.c_str(), data.length(), 0)) == data.length();
}

std::string Connection::recv()
{
    if(!valid) throw std::logic_error("Recv on Invalid Connection");

    std::string ret(Connection::MaxReceptionSize, '\0');

    ::recv(connection, ret.data(), ret.length(), 0);
    ret.erase(ret.find_first_of('\0'));

    return ret;
}

const std::string& Connection::ipaddr() const
{
    /*
    char ip[16] = {};
    ::inet_ntop(client.sin_family, &client.sin_addr, ip, std::size(ip));
    // ::inet_ntoa(client.sin_addr);
    return std::string (ip);
    */
    return ip;
}
