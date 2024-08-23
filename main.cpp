#include <iostream>
#include <filesystem>

#include "misc.hpp"
#include "Utility.hpp"
#include "Server.hpp"

int main()
{
    // serve(fs::current_path());

    // Socket socket(1337);
    http::Server server(1337);

    auto request = server.GetRequest();
    std::cout << "\n--------REQUEST BEG--------\n" << request << "\n--------REQUEST END--------\n";

    std::cout << "Method: " << http::utils::to_string(request.getMethod()) << '\n';
    std::cout << "Requested File: " << request.getFileName() << '\n';
    std::cout << "Http Protocol: " << http::http_versions.get_value(request.getVersion()) << std::endl;

    auto response = create_dummy_response(200);
    // con.send(response);
}
