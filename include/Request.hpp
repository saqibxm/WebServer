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

    const std::string& getHostName() const;
    http::Method getMethod() const;
    std::string getFileName() const;
    http::Version getVersion() const;
    const std::vector<std::string>& getMimes() const;

    std::ostream& print(std::ostream&) const;

private:
    std::string plain;
    std::string host; // the hostname
    bool valid; // generic : represents validity of the request

    void parse(); // parse the raw string
    http::Method method;
    fs::path file; // reqested file
    http::Version version; // version of protocol

    std::vector<std::string> mimes;
    std::multimap<std::string, std::vector<std::string>> headers; // or can use unordered map
};

std::ostream& operator << (std::ostream&, const Request&);

}

#endif // REQUEST_HPP
