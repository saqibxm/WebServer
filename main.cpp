#include <iostream>
#include <filesystem>

#include "misc.hpp"
#include "Socket.hpp"
#include "Utility.hpp"

int main()
{
    // serve(fs::current_path());

    Socket socket(1337);

    auto con = socket.Listen();

    std::cout << "Client Connected: " << con.get_ipaddr() << '\n';
    auto request = con.recv();
    std::cout << "\n--------REQUEST BEG--------\n" << request << "\n--------REQUEST END--------\n";

    auto response = create_dummy_response(200);
    con.send(response);
}
