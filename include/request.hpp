#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <map>
#include <vector>
#include "../utils.hpp"
#include "../include/server.hpp"


enum {
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NO_CONTENT = 204,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    TEMPORARY_REDIRECT = 307,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    REQUEST_TIMEOUT = 408,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

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
    Location _location;
    Server _server;
    std::string index;
    bool is_autoindex;
    int status_code;

public:
    Request();
    void parseRequest(std::string req);
    Request(std::string req, Web *web);
    void checkServer(Web *web);
    void checkLocation();
    ~Request();
};
#endif