#include "../include/cgi.hpp"


Cgi::Cgi() {};
Cgi::~Cgi() {};

void Cgi::initial_env_file(){
    this->env_map["SERVER_SOFTWARE"]="";
    this->env_map["SERVER_NAME"]="";
    this->env_map["SERVER_PROTOCOL"]="";
    this->env_map["SERVER_PORT"]="";
    this->env_map["REQUEST_METHOD"]="";
    this->env_map["PATH_INFO"]="";
    this->env_map["GATEWAY_INTERFACE"]="";
    this->env_map["PATH_TRANSLATED"]="";
    this->env_map["SCRIPT_NAME"]="";
    this->env_map["QUERY_STRING"]="";
    this->env_map["REMOTE_HOST"]="";
    this->env_map["REMOTE_ADDR"]="";
    this->env_map["CONTENT_LENGTH"]="";
    this->env_map["REDIRECT_STATUS"] = "";
    this->env_map["CONTENT_TYPE"] = "";
    this->env_map["PATH_TRANSLATED"] = "";
    this->env_map["AUTH_TYPE"] = "";
    this->env_map["REMOTE_USER"] = "";
    ////////////////////////////
    this->env_map["SERVER_PROTOCOL"] = "";
    this->env_map["HTTP_ACCEPT"] = "";
    this->env_map["HTTP_COOKIE"] = "";
    this->env_map["HTTP_ACCEPT_LANGUAGE"] = "";
    this->env_map["HTTP_REFERER"] = "";
    this->env_map["HTTP_USER_AGENT"] = "";
    this->env_map["HTTP_ACCEPT_ENCODING"] = "";
    this->env_map["HTTP_ACCEPT_CHARSET"] = "";
    this->env_map["DOCUMENT_ROOT"] = "";
    this->env_map["REQUEST_URI"] = "";
    this->env_map["UPLOAD_PATH"] = "";
}
Cgi::Cgi(Request req, std::string file)
{
    (void)req;
    (void)file;
    // this->initial_env_file();
    // this->_envMap(req, file);
    // this->body = req.body;
}
void Cgi::_envMap(Request req, std::string file)
{
    this->env_map["QUERY_STRING"]=req.query_params;
    this->env_map["CONTENT_TYPE"]=req.headers.find("Content-Type")->second;
    this->env_map["SERVER_NAME"]="webserv";
    this->env_map["REDIRECT_STATUS"] = "200";
	this->env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->env_map["SERVER_PORT"]=req.port+"";
    this->env_map["CONTENT_LENGTH"]=req.headers.find("Content-Length")->second!="" ? req.headers.find("Content-Length")->second : "";
    this->env_map["REMOTE_HOST"]=req.host;
    this->env_map["REQUEST_METHOD"]=req.method;
    this->env_map["SCRIPT_NAME"]=file;
    this->env_map["PATH_INFO"]=req.is_directory_file.second.substr(req.root.length());
    this->env_map["PATH_TRANSLATED"]=req.is_directory_file.second.substr(req.root.length());
    this->env_map["AUTH_TYPE"] = "Basic";
    this->env_map["REMOTE_USER"] = req.headers.find("From")->second !="" ? req.headers.find("From")->second : "";
    this->env_map["SERVER_PROTOCOL"] = req.version;
    this->env_map["SERVER_SOFTWARE"] = "webserv/1.0";
    this->env_map["HTTP_ACCEPT"] = req.headers.find("Accept")->second!="" ? req.headers.find("Accept")->second : "";
    this->env_map["HTTP_COOKIE"] = req.headers.find("Cookie")->second!="" ? req.headers.find("Cookie")->second : "";
    this->env_map["HTTP_ACCEPT_LANGUAGE"] = req.headers.find("Accept-Language")->second!="" ? req.headers.find("Accept-Language")->second : "";
    this->env_map["HTTP_REFERER"] = req.headers.find("Referer")->second!= "" ? req.headers.find("Referer")->second : "";
    this->env_map["HTTP_USER_AGENT"] = req.headers.find("User-Agent")->second !="" ? req.headers.find("User-Agent")->second : "";
    this->env_map["HTTP_ACCEPT_ENCODING"] = req.headers.find("Accept-Encoding")->second !="" ? req.headers.find("Accept-Encoding")->second : "";
    this->env_map["HTTP_ACCEPT_CHARSET"] = req.headers.find("Accept-Charset")->second !="" ? req.headers.find("Accept-Charset")->second : "";
    this->env_map["DOCUMENT_ROOT"] = req.root;
    this->env_map["REQUEST_URI"] = req.path + req.query_params != "" ? "?" + req.query_params : "";
    this->env_map["UPLOAD_PATH"] = req._server.__attributes["upload_dir"][0] !="" ? req._server.__attributes["upload_dir"][0] : "";
}