#pragma once
#include <string>
#include <map>

class HttpResponse {
public:
    HttpResponse(int status_code, const std::string& status_message,
                 const std::map<std::string, std::string>& headers,
                 const std::string& body);

    int status_code() const { return status_code_; }
    const std::string& status_message() const { return status_message_; }
    const std::map<std::string, std::string>& headers() const { return headers_; }
    const std::string& body() const { return body_; }

private:
    int status_code_;
    std::string status_message_;
    std::map<std::string, std::string> headers_;
    std::string body_;
};