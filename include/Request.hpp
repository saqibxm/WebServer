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
    explicit Request(const std::string&) = default;

    http::Method getMethod() const;
    std::string getPath() const;
    http::Version getVersion() const;

private:
    std::string plain;
    bool contains;

    std::unordered_map<std::string, std::vector<std::string>> headers; // or can use multimap
};

}

#endif // REQUEST_HPP
