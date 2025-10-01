#include "http_client.hpp"
#include <iostream>

int main() {
    try {
        HttpClient client;

        HttpRequest req(HttpMethod::GET, "httpbin.org", "/get");
        HttpResponse resp = client.send(req);

        std::cout << "Status: " << resp.status_code() << " " << resp.status_message() << "\n";
        std::cout << "Body:\n" << resp.body() << std::endl;

        HttpRequest post_req(HttpMethod::POST, "httpbin.org", "/post",
                             {{"Content-Type", "application/json"}},
                             "{\"key\": \"value\"}");
        HttpResponse post_resp = client.send(post_req);

        std::cout << "POST Status: " << post_resp.status_code() << "\n";
        std::cout << "POST Body:\n" << post_resp.body() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}