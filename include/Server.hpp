#pragma once

#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.h"
#include "Socket.hpp"

namespace http
{

class Request;
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
     * Request Require();
     * bool Respond(const Response&);
     */

    void SetRoot(const std::string&);
    void SetPort(unsigned short);

private:
    Socket socket;
    fs::path root;
    bool running;

    static void ServerThread(Server *const);
};

}

#endif // SERVER_HPP
