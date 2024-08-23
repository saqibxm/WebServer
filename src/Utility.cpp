#include <fstream> // std::ifstream
#include <iostream> // std::cerr, std::cout
#include <iomanip> // std::quoted

#include "Utility.hpp"

#include "bimap.hpp"

using namespace http;

std::string http::utils::to_string(statcodes::Code code)
{
    using namespace http::statcodes;

    std::string str;
    switch(code)
    {
    case Info::Continue:
        str = "Continue"; break;
    case Info::Processing:
        str = "Processing"; break;

    case Success::OK:
        str = "OK"; break;
    case Success::Created:
        str = "Created"; break;
    case Success::Accepted:
        str = "Accepted"; break;

    case Redirect::MultipleChoices:
        str = "Multiple Choices"; break;
    case Redirect::MovedPermanently:
        str = "Moved Permanently"; break;
    case Redirect::Found:
        str = "Found"; break;

    case CError::BadRequest:
        str = "Bad Request"; break;
    case CError::Unauthorized:
        str = "Unauthorized"; break;
    case CError::Forbidden:
        str = "Forbidden"; break;
    case CError::NotFound:
        str = "Not Found"; break;
    case CError::RequestTimeout:
        str = "Request Timeout"; break;
    }

    return str;
}

std::string http::utils::to_string(http::Method method)
{
    std::string str;

    switch (method) {
    case Method::Get:
        str = "GET";
        break;
    case Method::Post:
        str = "POST"; break;
    case Method::Update:
        str = "UPDATE"; break;
    case Method::Patch:
        str = "PATCH"; break;
    case Method::Delete:
        str = "Delete"; break;
    default:
        break;
    }

    return str;
}

std::optional<std::string> http::utils::get_file_contents(const fs::path &filename)
{
    std::ifstream file(filename, std::ifstream::binary);
    if(!fs::exists(filename) || !file.is_open()) {
        std::cerr << "File can't be opened: " << std::quoted(filename.string()) << '\n';
        return std::nullopt;
    }
    std::string ret(std::istreambuf_iterator<std::string::value_type>{file}, {});
    return ret;
}

std::string create_dummy_response(int status, std::string body)
{
    /*
     * 1. Status line.
     * 2. Zero or more headers, each ending with a CRLF.
     * 3. Optional response body.
     */

    std::string response = "HTTP/1.1";
    response.append(" " + std::to_string(status));
    response.append(" " + utils::to_string(status) + CRLF);
    response.append("Connection: Keep-Alive\n");
    response.append("Content-Type: text/html; charset=utf-8\n");
    response.append("Keep-Alive: timeout=5, max=10\n");
    if(!response.empty()) response.append(body);
    response.append(CRLF);
    return response;
}

/* OBJECT DEFINITIONS */
const stde::bimap<http::Version, std::string> http::http_versions = {
    { Version::HTTP_0_9, "HTTP/0.9" },
    { Version::HTTP_1_0, "HTTP/1.0" },
    { Version::HTTP_1_1, "HTTP/1.1" },
    { Version::HTTP_2_0, "HTTP/2.0" }
};
