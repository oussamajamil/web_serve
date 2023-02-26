#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <map>
#include <vector>
#include "utils.hpp"

class Request
{
private:
    std::string request;

public:
    std::vector<std::string> vec;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> body;
    std::string method;
    std::string path;
    std::string version;

public:
    Request();
    Request(std::string req);
};
#endif