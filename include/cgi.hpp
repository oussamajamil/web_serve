
#ifndef CGI_HPP
#define CGI_HPP

#include "./request.hpp"

class Cgi
{

private:
std::map <std::string,std::string> env_map;
void _envMap (Request req, std::string file);
void initial_env_file();


public:
    std::string body;
    Cgi();
    Cgi(Request req, std::string file);
    ~Cgi();
};

#endif

