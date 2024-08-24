#include <iostream>
#include <filesystem>

#include "misc.hpp"
#include "Utility.hpp"
#include "Server.hpp"
#include "Parser.hpp"

int main()
{
    // serve(fs::current_path());

    // Socket socket(1337);
    http::Server server(1337);

    auto request = server.GetRequest();
    std::cout << "\n--------REQUEST BEG--------\n" << request << "\n--------REQUEST END--------\n";

    auto [host, port] = request.get_hostname();
    std::cout << "Method: " << http::utils::to_string(request.get_method()) << '\n';
    std::cout << "Requested File: " << request.get_filename() << '\n';
    std::cout << "Hostname: " << host << ':' << port << '\n';
    std::cout << "Connection Type: " << http::to_string(request.get_contype()) << '\n';
    std::cout << "Http Protocol: " << http::http_versions.get_value(request.get_version()) << std::endl;

    auto ret = http::Parser::parse_requestline(request.get_plaintext());
    auto mimes = request.get_mimes();
    std::cout << "Mimes: ";
    for(const auto &mime : mimes) std::cout << mime << " | ";
    auto response = create_dummy_response(200);
    // con.send(response);
}
