#include "http_request.hpp"
#include <sstream>

HttpRequest::HttpRequest(HttpMethod method, const std::string& host, const std::string& path,
                         const std::map<std::string, std::string>& headers, const std::string& body)
    : method_(method), host_(host), path_(path), port_(80), headers_(headers), body_(body) {}

std::string HttpRequest::build_request() const {
    std::ostringstream req;

    if (method_ == HttpMethod::GET) {
        req << "GET " << path_ << " HTTP/1.1\r\n";
    } else if (method_ == HttpMethod::POST) {
        req << "POST " << path_ << " HTTP/1.1\r\n";
    }

    req << "Host: " << host_ << "\r\n";
    req << "Connection: close\r\n";

    for (const auto& [key, value] : headers_) {
        req << key << ": " << value << "\r\n";
    }

    if (method_ == HttpMethod::POST && !body_.empty()) {
        req << "Content-Length: " << body_.size() << "\r\n";
        req << "\r\n";
        req << body_;
    } else {
        req << "\r\n";
    }

    return req.str();
}