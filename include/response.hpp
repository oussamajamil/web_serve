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

class Response
{
private:
    std::string header;
    std::string body;;
    std::map<std::string,std::string> content_type;
    std::string path;
public:
    Response();
    std::string response_message;
    ~Response();
    Response(bool is_autoindex, std::string version,std::string path,std::string root, std::map<int, std::string> error_page_map ,int status_code,std::pair<bool, std::string> is_directory_file,std::map<std::string,std::string>body_form_data,std::string method,std::string redirect);
    std::string default_error_page(int status_code);
    std::string get_message(int status_code);
    std::string error_page(int status_code,std::map<int,std::string>error,std::string root);
    std::string get_file(std::string path);
    std::string generate_response(std::string version,int code,std::string body,std::string file);
    std::string generate_response_redirect(std::string version,int code,std::string redirect);
    void handle_content_type();
    std::string get_file_extencion(std::string file);
    std::string default_delete_page();
    std::string default_post_page(std::map<std::string,std::string>body_form_data);
}; 

#endif