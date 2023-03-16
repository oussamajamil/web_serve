#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <map>
#include <vector>
#include "../utils.hpp"
#include "../include/server.hpp"

class Request
{
private:
    std::string request;

public:
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> query_params;
    std::string body;
    std::string port;
    std::string host;

public:
    Request();
    Request(std::string req);
    bool is_valid();
    void checkServer(Web *web);
};
#endif