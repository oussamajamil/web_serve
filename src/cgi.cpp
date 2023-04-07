#include "../include/cgi.hpp"


Cgi::Cgi() {};
Cgi::~Cgi() {};

void Cgi::initial_env_file(){
    this->initial_env_file["SERVER_SOFTWARE"]="";
    this->initial_env_file["SERVER_NAME"]="";
    this->initial_env_file["SERVER_PROTOCOL"]="";
    this->initial_env_file["SERVER_PORT"]="";
    this->initial_env_file["REQUEST_METHOD"]="";
    this->initial_env_file["PATH_INFO"]="";
    this->initial_env_file["PATH_TRANSLATED"]="";
    this->initial_env_file["SCRIPT_NAME"]="";
    this->initial_env_file["QUERY_STRING"]="";
    this->initial_env_file["REMOTE_HOST"]="";
    this->initial_env_file["REMOTE_ADDR"]="";
    this->initial_env_file["CONTENT_LENGTH"]="";
}

void::_envMap(Request req, std::string file)
{
    this->initial_env_file["QUERY_STRING"]=req.query_params;
    this->initial_env_file["SERVER_NAME"]="webserv";
    this->initial_env_file["SERVER_PORT"]=req.port;
    if(std::atoi(req.headers.find("Content-Length")->second) >0)
        this->initial_env_file["CONTENT_LENGTH"]=req.headers.find("Content-Length")->second; 
    this->initial_env_file["REMOTE_HOST"]=req.host;
    this->initial_env_file["REQUEST_METHOD"]=req.method;
    this->initial_env_file["SCRIPT_NAME"]=file;
    this->initial_env_file["PATH_INFO"]=file.substr(file.find_last_of("/")+1);



}