#include "../include/response.hpp"
#include "../include/request.hpp"


Response::Response()
{
}
Response::~Response(){
}

void Response::handle_content_type(){
    this->content_type["html"] = "text/html";
    this->content_type["css"] = "text/css";
    this->content_type["js"] = "application/javascript";
    this->content_type["jpg"] = "image/jpeg";
    this->content_type["jpeg"] = "image/jpeg";
    this->content_type["png"] = "image/png";
    this->content_type["gif"] = "image/gif";
    this->content_type["svg"] = "image/svg+xml";
    this->content_type["mp4"] = "video/mp4";
    this->content_type["mp3"] = "audio/mpeg";
    this->content_type["json"] = "application/json";
    this->content_type["pdf"] = "application/pdf";
    this->content_type["txt"] = "text/plain";
    this->content_type["xml"] = "application/xml";
    this->content_type["zip"] = "application/zip";
    this->content_type["rar"] = "application/x-rar-compressed";
    this->content_type["gz"] = "application/gzip";
    this->content_type["tar"] = "application/x-tar";
    this->content_type["doc"] = "application/msword";
    this->content_type["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    this->content_type["xls"] = "application/vnd.ms-excel";
    this->content_type["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    this->content_type["ppt"] = "application/vnd.ms-powerpoint";
    this->content_type["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    this->content_type["odt"] = "application/vnd.oasis.opendocument.text";
    this->content_type["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    this->content_type["odp"] = "application/vnd.oasis.opendocument.presentation";
    this->content_type["odg"] = "application/vnd.oasis.opendocument.graphics";
    this->content_type["odc"] = "application/vnd.oasis.opendocument.chart";
    this->content_type["odf"] = "application/vnd.oasis.opendocument.formula";
    this->content_type["odg"] = "application/vnd.oasis.opendocument.graphics";
    this->content_type["odp"] = "application/vnd.oasis.opendocument.presentation";
	this->content_type["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	this->content_type["odt"] = "application/vnd.oasis.opendocument.text";
	this->content_type["rtf"] = "application/rtf";
	this->content_type["wma"] = "audio/x-ms-wma";
	this->content_type["wmv"] = "video/x-ms-wmv";
	this->content_type["webm"] = "video/webm";
	this->content_type["flv"] = "video/x-flv";
	this->content_type["avi"] = "video/x-msvideo";
	this->content_type["swf"] = "application/x-shockwave-flash";
	this->content_type["exe"] = "application/x-msdownload";
	this->content_type["msi"] = "application/x-msdownload";
	this->content_type["cab"] = "application/vnd.ms-cab-compressed";
	this->content_type["deb"] = "application/x-debian-package";
	this->content_type["gz"] = "application/x-gzip";
	this->content_type["rar"] = "application/x-rar-compressed";
	this->content_type["iso"] = "application/x-iso9660-image";
}

std::string  Response::get_message(int code){
    if(code== BAD_REQUEST)
        return "Bad Request";
    else if (code == METHOD_NOT_ALLOWED)
        return "Method Not Allowed";
    else if (code == NOT_FOUND)
        return "Not Found";
    else if (code ==REQUEST_ENTITY_TOO_LARGE)
        return "Request Entity Too Large";
    else if (code == NOT_IMPLEMENTED)
        return "Not Implemented";
    else if (code == HTTP_VERSION_NOT_SUPPORTED)
        return "HTTP Version Not Supported";
    else if (code == METHOD_NOT_ALLOWED)
        return "Method Not Allowed";
    else if (code == FORBIDDEN)
        return "Forbidden";
    else if (code == CREATED)
        return "Created";
    else if (code == ACCEPTED)
        return "Accepted";
    else if (code == NO_CONTENT)
        return "No Content";
    else if (code == MOVED_PERMANENTLY)
        return "Moved Permanently";
    else if (code == FOUND)
        return "Found";
    else if (code == SEE_OTHER)
        return "See Other";
    else if (code == NOT_MODIFIED)
        return "Not Modified";
    else if(code == TEMPORARY_REDIRECT)
        return "Temporary Redirect";
    else if (code == UNAUTHORIZED)
        return "Unauthorized";
    else if (code == INTERNAL_SERVER_ERROR)
        return "Internal Server Error";
    else if (code == BAD_GATEWAY)
        return "Bad Gateway";
    else if( code == SERVICE_UNAVAILABLE)
        return "Service Unavailable";
    else if (code == GATEWAY_TIMEOUT)
        return "Gateway Timeout";
    else 
        return "OK";
}
std::string Response::default_error_page(int code)
{
    return std::string("<html><head><title>") + std::to_string(code) + std::string(" ") + get_message(code) + std::string("</title></head><body><center><h1>") + std::to_string(code) + std::string(" ") + get_message(code) + std::string("</h1></center><hr><center>nginx</center></body></html>");
}
std::string Response::get_file_extencion(std::string file){
   
    return file.substr(file.find_last_of(".") + 1);
}
std::string Response::generate_response(std::string version,int code, std::string body,std::string file){
    std::string response = version + " " + std::to_string(code) + " " + get_message(code);
    response += "\r\n";
    response+="Server: " + "web_serve" + "\r\n";
    response += "Content-Length: " + std::to_string(body.length()) + "\r\n";

    if (file != "")
    response += "Content-Type: "+this->content_type[get_file_extencion(file)] + "\r\n";
    else{
        response += "Content-Type: text/html\r\n";
    }
    response += "\r\n";
    response += body;
    return response;
}

std::string autoIndexPage(std::string path)
{
    std::string res = "<html><head><title>Index of " + path + "</title></head><body bgcolor=\"white\"><h1>Index of " + path + "</h1><hr><pre><a href=\"../\">../</a>";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] != '.')
            {
                res += "<a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a>";
            }
        }
        closedir(dir);
    }
    else
    {

    }
    res += "</pre><hr></body></html>";
    return res;
}

std::string Response::get_file(std::string path){
    std::ifstream file (path);
    std::string res;
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            res += line;
        }
        file.close();
    }
    return res; 
}

std::string Response::error_page(int status_code,std::map<int,std::string>error,std::string root){
    std::string res;
    if (error.find(status_code) != error.end())
    {
        if(file_exists(root + error[status_code]))
        {
            this->path = root + error[status_code];
            res =  get_file(this->path);
        }
    }
    else
    {
        this->path = "";
        res=default_error_page(status_code);
    }
    return res;
}

std::string Response::default_delete_page(){
    std::string res = "<html><head><title>Index of " + path + "</title></head><body bgcolor=\"white\"><h1>delete path</h1>>";
    res += "</pre><hr></body></html>";
    return res;
}

std::string Response::default_post_page(std::map<std::string,std::string>body_form_data){
    std::string res = "<html><head><title>Index of " + path + "</title></head><body bgcolor=\"white\"><h1>Index of " + path + "</h1><hr><pre>";
    std::map<std::string,std::string>::iterator it;
    for (it = body_form_data.begin(); it != body_form_data.end(); it++)
    {
        res += it->first + " : " + it->second + "\n";
    }
    res += "</pre><hr></body></html>";
    return res;
}
std::string Response::generate_response_redirect(std::string version,int code,std::string redirect){
    std::string response = version + " " + std::to_string(code) + " " + get_message(code);
    response+= "\r\n";
    response += "Connection: "+ Request::_connection + "\r\n";
    response += "Date: " + get_date() + "\r\n";
    response += "Server: web_serve"  + "\r\n";
    response += "\r\n";
    response += "Location: " + redirect + "\r\n";
    response += "\r\n";
    response += redirect;
    return response;
}
Response::Response(bool is_autoindex, std::string version,std::string path,std::string root, std::map<int, std::string> error_page_map ,int status_code,std::pair<bool, std::string> is_directory_file,std::map<std::string,std::string>body_form_data,std::string method,std::string redirect){
    (void)path;
    handle_content_type();
    if (redirect != "")
    {
            this->response_message = generate_response_redirect(version,OK,redirect);
            std::cout <<
            return;
    }
    if(status_code != OK)
    {
        this->body = error_page(status_code,error_page_map,root);
        this->response_message = generate_response(version,status_code,this->body,this->path);
        return;
    }
    else
    {
        if(method == "DELETE"){
            this->body = default_delete_page();
            this->response_message = generate_response(version,OK,this->body,"");
            return;
        }
        else if(method == "POST"){
            this->body = default_post_page(body_form_data);
            this->response_message = generate_response(version,OK,this->body,"");
            return;
        }
         else 
        {
      
            if (is_directory_file.first){
                if (is_autoindex)
                {
                    this->body = autoIndexPage(is_directory_file.second);
                    this->response_message = generate_response(version,OK,this->body,"");
                    return;
                }
                else
                {
                    this->body = error_page(403,error_page_map,root);
                    this->response_message = generate_response(version,403,this->body,this->path);
                    return;
                }
            }
            else
            {
                this->body = get_file(is_directory_file.second);
                this->response_message = generate_response(version,status_code,this->body,is_directory_file.second);
                return;
            }
        }
    }
    std::cout << "respone: " << this->response_message << std::endl;
}
