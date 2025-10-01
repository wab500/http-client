#include "socket_connection.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

SocketConnection::SocketConnection(const std::string& host, int port)
    : host_(host), port_(port), sockfd_(-1) {}

SocketConnection::~SocketConnection() {
    if (sockfd_ != -1) {
        close(sockfd_);
    }
}

void SocketConnection::connect() {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = std::to_string(port_);
    int status = getaddrinfo(host_.c_str(), port_str.c_str(), &hints, &res);
    if (status != 0) {
        throw std::runtime_error("getaddrinfo: " + std::string(gai_strerror(status)));
    }

    sockfd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd_ == -1) {
        freeaddrinfo(res);
        throw std::runtime_error("socket() failed");
    }

    if (::connect(sockfd_, res->ai_addr, res->ai_addrlen) == -1) {
        close(sockfd_);
        sockfd_ = -1;
        freeaddrinfo(res);
        throw std::runtime_error("connect() failed");
    }

    freeaddrinfo(res);
}

void SocketConnection::send(const std::string& data) {
    if (send(sockfd_, data.c_str(), data.size(), 0) == -1) {
        throw std::runtime_error("send() failed");
    }
}

std::string SocketConnection::receive() {
    char buffer[4096];
    std::string response;
    ssize_t bytes;

    while ((bytes = recv(sockfd_, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes] = '\0';
        response += buffer;
    }

    if (bytes == -1) {
        throw std::runtime_error("recv() failed");
    }

    return response;
}