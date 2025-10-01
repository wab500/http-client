#include "http_client.hpp"
#include "socket_connection.hpp"
#include <sstream>
#include <stdexcept>

class SocketConnectionAdapter : public IConnection {
public:
    SocketConnectionAdapter(const std::string& host, int port)
        : conn_(std::make_unique<SocketConnection>(host, port)) {}

    void connect() override { conn_->connect(); }
    void send(const std::string& data) override { conn_->send(data); }
    std::string receive() override { return conn_->receive(); }

private:
    std::unique_ptr<SocketConnection> conn_;
};

HttpClient::HttpClient(std::unique_ptr<IConnection> connection)
    : connection_(std::move(connection)) {}

HttpResponse HttpClient::send(const HttpRequest& request) {
    if (!connection_) {
        connection_ = std::make_unique<SocketConnectionAdapter>(request.host(), request.port());
    }

    connection_->connect();
    connection_->send(request.build_request());
    std::string raw_response = connection_->receive();

    size_t header_end = raw_response.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        throw std::runtime_error("Invalid HTTP response: no header/body separator");
    }

    std::string headers_part = raw_response.substr(0, header_end);
    std::string body = raw_response.substr(header_end + 4);

    std::istringstream header_stream(headers_part);
    std::string status_line;
    std::getline(header_stream, status_line);

    std::istringstream status_stream(status_line);
    std::string http_version, status_code_str, status_message;
    status_stream >> http_version >> status_code_str >> status_message;

    int status_code = std::stoi(status_code_str);

    std::map<std::string, std::string> headers;
    std::string line;
    while (std::getline(header_stream, line)) {
        if (line.empty()) continue;
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            // Убираем начальные пробелы
            value.erase(0, value.find_first_not_of(" \t"));
            headers[key] = value;
        }
    }

    return HttpResponse(status_code, status_message, headers, body);
}