#pragma once
#include <string>
#include <map>

enum class HttpMethod {
    GET,
    POST
};

class HttpRequest {
public:
    HttpRequest(HttpMethod method, const std::string& host, const std::string& path = "/",
                const std::map<std::string, std::string>& headers = {},
                const std::string& body = "");

    std::string build_request() const;

    HttpMethod method() const { return method_; }
    const std::string& host() const { return host_; }
    const std::string& path() const { return path_; }
    int port() const { return port_; }

private:
    HttpMethod method_;
    std::string host_;
    std::string path_;
    int port_;
    std::map<std::string, std::string> headers_;
    std::string body_;
};