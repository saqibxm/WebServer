#include "Request.hpp"
#include "Parser.hpp"
#include <charconv>
#include <array>
#include <iostream>
#include <regex>

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

std::pair<std::string, unsigned short> Request::get_hostname() const
{
    auto sep = host.find(':') + 1;
    unsigned short port = 0;

    auto [ptr, ec] = std::from_chars(host.c_str() + sep, host.c_str() + host.length() - sep, port);
    if(ec == std::errc::invalid_argument) port = -1;
    return { host.substr(0, sep - 1), port};
}

const std::vector<std::string>& Request::get_mimes() const
{
    return mimes;
}

http::Method Request::get_method() const
{
    return method;
}

std::string Request::get_filename() const
{
    if(!uri.has_filename()) return "/";
    return uri.filename();
}

http::Version Request::get_version() const
{
    return version;
}

http::ConnectionType Request::get_contype() const
{
    return conn;
}

const std::string& Request::get_plaintext() const
{
    return plain;
}

std::ostream& Request::print(std::ostream &os) const
{
    return os << plain;
}

void Request::parse()
{
    if(!valid) throw std::invalid_argument("Can't Parse Invalid Request!");

    // std::string methodstr, pathstr, protostr;
    // std::vector<std::string> tokens;
    // std::array<std::string, 3> tokens;
    /* marker = reqline.find_first_of(' ');
    do {
        tokens.emplace_back(reqline, last, marker - last);
        last = ++marker;
    } while((marker = reqline.find_first_of(' ', marker)) != std::string::npos);

    for(decltype(tokens)::size_type i = 0; i < tokens.size(); ++i)
    {
        marker = reqline.find_first_of(' ', marker);
        tokens[i].assign(reqline, last, marker - last);
        last = ++marker;
    } */

    method = Parser::parse_method(plain);
    uri = Parser::parse_uri(plain);
    version = Parser::parse_version(plain);

    if(method == Method::Invalid) throw std::domain_error("Possibly Missing Request Line!");
    // host = Parser::parse_hostname(plain).value_or("Unknown:???");

    parse_headers();
    if(auto it = headers.find("Connection"); it != headers.end())
        conn = http::utils::to_connection(it->second);
    else conn = ConnectionType::Close;

    if(auto it = headers.find("Host"); it != headers.end())
        host = it->second;
    else host = "Unknown:???";

    parse_mimes();
}

void Request::parse_headers()
{
    std::string headers(plain, plain.find(CRLF) + 2, plain.rfind(CRLF));

    std::string::size_type pos = 0, mark = 0;

    while((pos = headers.find_first_of(':', pos)) != std::string::npos)
    {
        std::string key = headers.substr(mark, pos - mark);

        auto beg = headers.find_first_not_of(WS, ++pos);
        auto end = headers.find(CRLF, beg);
        // if(end == std::string::npos) end = headers.find_first_of(LF, beg);

        this->headers.insert({key, headers.substr(beg, end - beg)});
        pos = mark = end + 2;
    }
}

void Request::parse_mimes()
{
    if(!valid || headers.empty()) throw std::invalid_argument("Request is Invalid or Headers are not Processed");

    // for pattern explaination visit regex101.com and paste this pattern alongwith sample
    // R"(((?:[a-z]{4,12}|\*)\/(?:\*|[^,;]{3,15}))[,;\s]?)";
    static const std::string mime_pattern = R"(((?:[a-z]{4,12}|\*)\/(?:\*|[^,;]{3,15})))";
    static std::regex mime_regex(mime_pattern, std::regex::ECMAScript | std::regex::icase | std::regex::optimize);

    if(!headers.count("Accept")) return; // no mimes found in headers
    std::smatch match;
    for(auto [beg, end] = headers.equal_range("Accept"); beg != end; ++beg)
    {
        const std::string &mimes = beg->second;
        for(std::sregex_iterator it(mimes.begin(), mimes.end(), mime_regex), it_end;
            it != it_end; ++it)
        {
            this->mimes.push_back(it->str());
        }
    }
}

std::ostream& http::operator << (std::ostream &os, const Request &obj)
{
    std::ostream::iostate old_state = os.rdstate();
    os.clear();

    obj.print(os);

    os.clear(old_state);
    return os;
}

/*
    std::string::size_type start = 0, current = 0;
    while((current = mimes.find(';', current)) != std::string::npos)
    {
        std::string line(mimes, start, current - start);

    }
*/
