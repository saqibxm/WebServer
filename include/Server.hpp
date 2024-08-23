#pragma once

#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.h"
#include "Socket.hpp"
#include "Request.hpp"

namespace http
{

constexpr unsigned short HttpPort = 80;

class Response;

class Server
{
public:
    Server();
    Server(unsigned short port);
    ~Server();

    Request GetRequest();
    bool SendResponse(const Response&);

    /*
     * Request Required();
     * bool Respond(const Response&);
     */

    void SetRoot(const fs::path&);
    bool SetPort(unsigned short);

private:
    Socket socket;
    fs::path root;
    bool running;

    static void ServerThread(Server *const);
};

}

#endif // SERVER_HPP
