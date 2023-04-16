#ifndef CGI_HPP
#define CGI_HPP

#include "./request.hpp"
#include <fcntl.h>

#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
#include <map>


class Cgi
{

private:

int  allocSize;
// char **_env;
// std::string     body;
std::string     path;
std::string cgiInput;
std::map <std::string,std::string> env_map;

void _envMap (Request req, std::string cgi_filePath,std::string file);
void initial_env_file();
std::string _getInput(std::string file);
void setEnv();
void setCgiRespoHeaders(std::vector<std::string> &str);
void setCgiRespoBody(std::vector<std::string> &str);
std::string get_input(std::string file);


public:

    Cgi();
    Cgi(Request req, std::string file);
    ~Cgi();
    char **_env;
    std::string     results;
    int execute(Request req, std::string cgi_filePath, std::string file);
};

#endif