#pragma once
#include "http_request.hpp"
#include "http_response.hpp"
#include <memory>

// Интерфейс для внедрения зависимости (для тестирования)
class IConnection {
public:
    virtual ~IConnection() = default;
    virtual void connect() = 0;
    virtual void send(const std::string& data) = 0;
    virtual std::string receive() = 0;
};

class HttpClient {
public:
    // Для тестов: можно передать мок-соединение
    explicit HttpClient(std::unique_ptr<IConnection> connection = nullptr);

    HttpResponse send(const HttpRequest& request);

private:
    std::unique_ptr<IConnection> connection_;
};