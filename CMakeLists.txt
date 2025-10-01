cmake_minimum_required(VERSION 3.14)
project(http_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(http_client
        src/main.cpp
        src/http_client.cpp
        src/socket_connection.cpp
)

target_include_directories(http_client PRIVATE include)
target_link_libraries(http_client)