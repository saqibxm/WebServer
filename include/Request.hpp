#pragma once

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <unordered_map>
#include <vector>

#include "common.h"
#include "Utility.hpp"

namespace http
{

class Request
{
public:
    explicit Request(const std::string&);
    void create(const std::string&);
    void clear(); // reset everything

    std::pair<std::string, unsigned short> get_hostname() const;
    http::Method get_method() const;
    std::string get_filename() const;
    http::Version get_version() const;
    http::ConnectionType get_contype() const;
    const std::vector<std::string>& get_mimes() const;
    const std::string& get_plaintext() const;

    std::ostream& print(std::ostream&) const;

private:
    std::string plain;
    std::string host; // the hostname
    bool valid; // generic : represents validity of the request

    void parse(); // parse the raw string
    void parse_mimes();
    void parse_headers();

    http::Method method;
    fs::path uri; // requested file
    http::Version version; // version of protocol
    http::ConnectionType conn = ConnectionType::Unknown;

    std::vector<std::string> mimes;
    std::multimap<std::string, std::string> headers; // or can use unordered map
};

// std::multimap<std::string, std::vector<std::string>> headers; // or can use unordered map

std::ostream& operator << (std::ostream&, const Request&);

}

#endif // REQUEST_HPP
