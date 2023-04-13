#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/request.hpp"
#include "../include/cgi.hpp"

class Response
{
private:
    std::string header;
    std::string body;
    std::map<std::string, std::string> content_type;
    std::string path;
    Cgi _cgi;


public:
    Response();
    std::string response_message;
    ~Response();
    Response(Request request);
    std::string default_error_page(int status_code);
    std::string get_message(int status_code);
    std::string error_page(int status_code, std::map<int, std::string> error, std::string root);
    std::string get_file(std::string path);
    std::string generate_response(Request *request);
    void handle_content_type();
    std::string get_file_extencion(std::string file);
    std::string default_delete_page();
    void clear();
    std::string default_post_page(std::map<std::string, std::string> body_form_data);
};

#endif