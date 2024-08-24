#include "Request.hpp"
#include "Parser.hpp"
#include <charconv>
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

std::pair<std::string, unsigned short> Request::getHostName() const
{
    auto sep = host.find(':') + 1;
    unsigned short port = 0;

    auto [ptr, ec] = std::from_chars(host.c_str() + sep, host.c_str() + host.length() - sep, port);
    if(ec == std::errc::invalid_argument) port = -1;
    return { host.substr(0, sep - 1), port};
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
    if(!uri.has_filename()) return "/";
    return uri.filename();
}

http::Version Request::getVersion() const
{
    return version;
}

http::ConnectionType Request::getConnectionType() const
{
    return conn;
}

const std::string& Request::getPlain() const
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

    if(method == Method::Invalid) valid = false;
    host = Parser::parse_hostname(plain).value_or("Unknown:???");

    parse_headers();
    if(auto it = headers.find("Connection"); it != headers.end())
        conn = http::utils::to_connection(it->second);
    else conn = ConnectionType::Close;
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

    if(!headers.count("Accept")) return; // no mimes found in headers
    for(auto [beg, end] = headers.equal_range("Accept"); beg != end; ++beg)
    {
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
