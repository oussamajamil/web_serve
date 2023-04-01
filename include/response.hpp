#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

class Response
{
private:
    std::string responseMessage;
public:
    response(bool is_autoindex,std::map<std::string,std::string>body_form_data,int status_code,std::string version,std::string path,std::string method,std::string host,std::string port,std::pair<bool,std::string>is_directory_file.std::map<int,std::string> page_error);
    ~response();
    std::string default_error_page(int status_code);
    std::string get_message(int status_code);
}; 

// void Request::handle_content_type()
// {
// 	this->content_type["html"] = "text/html";
// 	this->content_type["css"] = "text/css";
// 	this->content_type["js"] = "application/javascript";
// 	this->content_type["jpg"] = "image/jpeg";
// 	this->content_type["jpeg"] = "image/jpeg";
// 	this->content_type["png"] = "image/png";
// 	this->content_type["gif"] = "image/gif";
// 	this->content_type["svg"] = "image/svg+xml";
// 	this->content_type["mp4"] = "video/mp4";
// 	this->content_type["mp3"] = "audio/mpeg";
// 	this->content_type["json"] = "application/json";
// 	this->content_type["pdf"] = "application/pdf";
// 	this->content_type["txt"] = "text/plain";
// 	this->content_type["xml"] = "application/xml";
// 	this->content_type["zip"] = "application/zip";
// 	this->content_type["rar"] = "application/x-rar-compressed";
// 	this->content_type["gz"] = "application/gzip";
// 	this->content_type["tar"] = "application/x-tar";
// 	this->content_type["doc"] = "application/msword";
// 	this->content_type["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
// 	this->content_type["xls"] = "application/vnd.ms-excel";
// 	this->content_type["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
// 	this->content_type["ppt"] = "application/vnd.ms-powerpoint";
// 	this->content_type["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
// 	this->content_type["odt"] = "application/vnd.oasis.opendocument.text";
// 	this->content_type["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
// 	this->content_type["odp"] = "application/vnd.oasis.opendocument.presentation";
// 	this->content_type["odg"] = "application/vnd.oasis.opendocument.graphics";
// 	this->content_type["odc"] = "application/vnd.oasis.opendocument.chart";
// 	this->content_type["odf"] = "application/vnd.oasis.opendocument.formula";
// 	this->content_type["odg"] = "application/vnd.oasis.opendocument.graphics";
// 	this->content_type["odp"] = "application/vnd.oasis.opendocument.presentation";
// 	this->content_type["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
// 	this->content_type["odt"] = "application/vnd.oasis.opendocument.text";
// 	this->content_type["rtf"] = "application/rtf";
// 	this->content_type["wma"] = "audio/x-ms-wma";
// 	this->content_type["wmv"] = "video/x-ms-wmv";
// 	this->content_type["webm"] = "video/webm";
// 	this->content_type["flv"] = "video/x-flv";
// 	this->content_type["avi"] = "video/x-msvideo";
// 	this->content_type["swf"] = "application/x-shockwave-flash";
// 	this->content_type["exe"] = "application/x-msdownload";
// 	this->content_type["msi"] = "application/x-msdownload";
// 	this->content_type["cab"] = "application/vnd.ms-cab-compressed";
// 	this->content_type["deb"] = "application/x-debian-package";
// 	this->content_type["gz"] = "application/x-gzip";
// 	this->content_type["rar"] = "application/x-rar-compressed";
// 	this->content_type["iso"] = "application/x-iso9660-image";
// }

#endif