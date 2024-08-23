#include "Request.hpp"
#include <array>

using namespace http;

Request::Request(const std::string &raw) : plain(raw), valid(true)
{
    parse();
}

void Request::create(const std::string &raw)
{
    plain = raw;
    clear();
    parse();
}

void Request::clear()
{
    valid = false;
    headers.clear();
    mimes.clear();
    plain.clear();
    host.clear();
}

const std::string& Request::getHostName() const
{
    return host;
}

const std::vector<std::string>& Request::getMimes() const
{
    return mimes;
}

http::Method Request::getMethod() const
{
    return method;
}

std::string Request::getFileName() const
{
    if(!file.has_filename()) return "/";
    return file.filename();
}

http::Version Request::getVersion() const
{
    return version;
}

std::ostream& Request::print(std::ostream &os) const
{
    return os << plain;
}

void Request::parse()
{
    if(!valid) throw std::invalid_argument("Can't Parse Invalid Request!");

    std::string request = plain;

    std::string reqline = request.substr(0, request.find_first_of(CRLF)); // first line of request

    std::string::size_type last = 0, marker = 0;
    // std::string methodstr, pathstr, protostr;
    std::vector<std::string> tokens;

    while((marker = reqline.find_first_of(' ', marker)) != std::string::npos)
    {
        tokens.emplace_back(reqline, last, marker - last);
        last = ++marker;
    }
    tokens.emplace_back(reqline, last, marker - last);

    method = utils::to_method(tokens.at(0));
    file = tokens.at(1);
    version = http_versions.get_key(tokens.at(2));

    if(method == Method::Invalid) valid = false;
}

std::ostream& http::operator << (std::ostream &os, const Request &obj)
{
    auto old_state = os.rdstate();
    os.clear();

    return obj.print(os);
    os.setstate(old_state);
}
