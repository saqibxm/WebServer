#include "Parser.hpp"

namespace
{

std::size_t method_start = 0, method_size = 0;

}

namespace http::Parser
{

http::Method parse_method(const std::string &request)
{
    auto mark = request.find_first_of(' ');

    std::string method = request.substr(0, mark);
    return utils::to_method(method);
}

std::string parse_uri(const std::string &request)
{
    auto mark = request.find_first_of(' ') + 1;
    auto end = request.find_first_of(' ', mark);

    return request.substr(mark, end - mark);
}

http::Version parse_version(const std::string &request)
{
    auto end = request.find(CRLF);
    auto mark = request.find_last_of(' ', end) + 1;

    std::string version(request, mark, end - mark);
    if(!http::http_versions.has_value(version)) return http::Version::Unknown;
    return http::http_versions.get_key(version);
}

std::optional<std::string> parse_hostname(const std::string &request)
{
    return parse_header("Host", request);
}

std::array<std::string, ReqTokens> parse_requestline(const std::string &request)
{
    std::array<std::string, ReqTokens> ret;
    std::string line = std::string(request, 0, request.find(CRLF));
    auto last = 0UL, current = last;

    for(decltype(ret)::size_type i = 0; i < ret.size(); ++i)
    {
        current = line.find_first_of(WS, current);
        ret[i].assign(line, last, current - last);
        last = ++current;
    }
    return ret;
}

std::optional<std::string> parse_header(std::string key, const std::string &request)
{
    std::string ret;
    if(key.back() != ':') key.push_back(':');

    if(auto pos = request.find(key); pos == std::string::npos)
        return std::nullopt;
    else
    {
        auto end = request.find_first_of(CRLF, pos + key.length());
        auto beg = request.find_last_of(' ', end) + 1;

        ret = request.substr(beg, end - beg);
        // ret.assign(request, beg, end - beg);
    }
    return ret;
}

}
