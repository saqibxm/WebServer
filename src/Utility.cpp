#include <fstream> // std::ifstream
#include <iostream> // std::cerr, std::cout
#include <iomanip> // std::quoted
#include <cctype>
#include <algorithm>

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

http::Method http::utils::to_method(std::string method)
{
    std::transform(method.cbegin(), method.cend(), method.begin(), [] (decltype(method)::value_type c) {
        return std::toupper(c);
    });

    Method m = Method::Invalid;

    if(method == "GET")
        m = Method::Get;
    else if(method == "HEAD")
        m = Method::Head;
    else if(method == "PUT")
        m = Method::Put;
    else if(method == "UPDATE")
        m = Method::Update;
    else if(method == "POST")
        m = Method::Post;
    else if(method == "PATCH")
        m = Method::Patch;
    else if(method == "CONNECT")
        m = Method::Connect;
    else if(method == "DELETE")
        m = Method::Delete;
    else if(method == "CONNECT")
        m = Method::Connect;
    else if(method == "TRACE")
        m = Method::Trace;
    else;

    return m;
}

std::string http::utils::to_string(ConnectionType type)
{
    std::string ret;

    switch(type)
    {
    case ConnectionType::Close:
        ret = "close"; break;
    case ConnectionType::KeepAlive:
        ret = "Keep-Alive"; break;
    default:
        break;
    }

    return ret;
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

std::string http::get_mime_type(const std::string &extension)
{
    auto it = mime_types.find(extension);
    if(it == mime_types.end()) return "unknown/???";

    return it->second;
}

std::string http::get_mime_type(const fs::path &filename)
{
    auto extension = filename.extension().string();
    extension = extension.substr(extension.find('.') + 1);

    return get_mime_type(extension);
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

const std::map<std::string_view, std::string> http::mime_types = {
    { "", "text/html" }, // default no-extension
    { "txt", "text/plain" },
    { "html", "text/html" },
    { "css", "text/css" },
    { "csv", "text/csv" },
    { "js", "text/javascript" },
    { "json", "text/json" },

    { "jpg", "image/jpeg" },
    { "jpeg", "image/jpeg" },
    { "png", "image/png", },
    { "bmp", "image/bmp" },
    { "gif", "image/gif" },
    { "webp", "image/webp" },
    { "ico", "image/vnd.microsoft.icon" },

    { "mp3", "audio/mpeg" },
    { "aac", "audio/aac" },
    { "opus", "audio/ogg" },

    { "mp4", "video/mp4" },
    { "mpeg", "video/mpeg" },

    { "ttf", "font/ttf" },
    { "xxx", "font/xxx" },

    { "zip", "application/zip" },
    { "pdf", "application/pdf" },
    { "rar", "application/vnd.rar" },
    { "bin", "application/octet-stream" }
};
