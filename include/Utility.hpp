#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <optional>
#include <type_traits>
#include <map>

#include "bimap.hpp"
#include "common.h"
// #include "tl/expected.hpp"

namespace http
{
enum class Version
{
    Unknown,
    HTTP_0_9,
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0
};

enum class Method
{
    Invalid,
    Get,
    Head,
    Put,
    Post,
    Patch,
    Update,
    Connect,
    Options,
    Trace,
    Delete
};

enum class ConnectionType
{
    Unknown,
    Close,
    Reuse,
    KeepAlive
};

namespace statcodes
{

enum Info
{
    Continue = 100,
    Processing = 102
};

enum Success
{
    OK = 200,
    Created = 201,
    Accepted = 202
};

enum Redirect
{
    MultipleChoices = 300,
    MovedPermanently = 301,
    Found = 302
};

enum CError
{
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    RequestTimeout = 408
};

enum SError
{
    InternalError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    Unavailable = 503,
    GatewayTimeout = 504
};

using Error = std::underlying_type_t<SError>;
using Code = std::underlying_type_t<Info>;

} /* statcodes */

inline namespace utils
{

std::string to_string(statcodes::Code code);
std::string to_string(http::Method method);
std::string to_string(http::ConnectionType type);

Method to_method(std::string);
ConnectionType to_connection(std::string);

std::optional<std::string> get_file_contents(const fs::path &);
std::string get_mime_type(const std::string&);
std::string get_mime_type(const fs::path&); // requires full filename / path

} /* utils */

extern const stde::bimap<Version, std::string> http_versions;
extern const std::map<std::string_view, std::string> mime_types;
} /* http */

inline void sleep_for_ms(unsigned ms)
{
#ifdef PLATFORM_POSIX
    usleep(ms * 1000);
#else
    Sleep(ms);
#endif
}

std::string create_dummy_response(int status, std::string body = "");

#endif // UTILS_HPP
