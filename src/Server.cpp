#include "Server.hpp"

using namespace http;

Server::Server() : Server(HttpPort)
{
    // ...
}

Server::Server(unsigned short port) : socket(port), root(fs::current_path()), running(socket.Opened())
{
    // ...
    // running = socket.Opened();
}

Server::~Server()
{
    ;
}

bool Server::SetPort(unsigned short port) // untested may not work
{
    running = false;
    socket.Switch(port);
    running = true;

    return socket.Opened();
}

void Server::SetRoot(const fs::path &path)
{
    if(!fs::is_directory(path)) throw std::invalid_argument("Given Path is not a directory!");
    root = path;
}
