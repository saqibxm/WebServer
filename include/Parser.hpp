#pragma once

#ifndef PARSER_HPP
#define PARSER_HPP

#include <optional>
#include <array>
#include "common.h"
#include "Request.hpp"
#include "Utility.hpp"

// namespace http { class Request; }

namespace { inline constexpr unsigned short ReqTokens = 3; }

namespace http::Parser
{

/*
http::Method parse_method(const Request&);
http::Version parse_version(const Request&);
std::optional<std::string> parse_hostname(const class http::Request&);
std::array<std::string, 3> parse_requestline(const Request&);

std::string parse_uri(const Request&);
std::string parse_header(std::string key, const Request&);
*/

http::Method parse_method(const std::string&);
http::Version parse_version(const std::string&);
std::optional<std::string> parse_hostname(const std::string&);
std::array<std::string, ReqTokens> parse_requestline(const std::string&);

std::string parse_uri(const std::string&);
std::optional<std::string> parse_header(std::string key, const std::string&);

}

namespace http::Parser::details
{

std::string transform_header_key(std::string_view); // transform key to appropriate format (Capitalize)

}

#endif // PARSER_HPP
