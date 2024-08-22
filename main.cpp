#include <iostream>
#include <filesystem>
#include "server.hpp"
#include "Socket.hpp"

int main()
{
    // serve(fs::current_path());

    Socket socket(1337);

    for(int i = 0; i < 5; ++i)
    {
    auto con = socket.Listen();

    std::cout << "Client Connected: " << con.get_ipaddr() << '\n';
    auto request = con.recv();
    std::cout << "\n--------REQUEST BEG--------\n" << request << "\n--------REQUEST END--------\n";

    auto response = CreateResponse(200);
    con.send(response);
    }
}
