#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <map>
#include <vector>
#include "../utils.hpp"
#include "../include/server.hpp"
#include "../include/receive.hpp"
#include <fstream>
#include <sys/stat.h>

typedef std::basic_ifstream<char> ifstream;

enum
{
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
    HTTP_VERSION_NOT_SUPPORTED = 505,
     REQUEST_ENTITY_TOO_LARGE = 413,
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
    std::string query_params;
    std::string body;
    std::string port;
    std::string host;
    Location _location;
    Server _server;
    bool is_autoindex;
    std::string responseMessage;
    int status_code;
    std::vector<std::string> body_boundary;
    std::pair<bool, std::string> is_directory_file;
    std::map<int, std::string> error_page_map;
    std::map<std::string, std::string> body_form_data;
public:
    Request();
    void parseRequest(std::string header, std::string body);
    Request(Receive *__r);
    void checkLocation();
    
    ~Request();
};
#endif