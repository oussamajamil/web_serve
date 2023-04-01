#include "../include/response.hpp"



Response::Response()
{
}

Response::~Response()
{
}

std::string  Response::get_message(int code){
    if(code== 400){
        return "Bad Request";
    }
}
std::string Response::default_error_page(int code)
{
    return std::string("<html><head><title>") + std::to_string(code) + std::string(" ") + get_error_message(code) + std::string("</title></head><body><center><h1>") + std::to_string(code) + std::string(" ") + get_message(code) + std::string("</h1></center><hr><center>nginx</center></body></html>");
}