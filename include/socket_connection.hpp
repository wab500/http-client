#pragma once
#include <string>

class SocketConnection {
public:
    explicit SocketConnection(const std::string& host, int port);
    ~SocketConnection();

    void connect();
    void send(const std::string& data);
    std::string receive();

private:
    std::string host_;
    int port_;
    int sockfd_;
};