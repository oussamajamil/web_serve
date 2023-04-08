#include "../include/response.hpp"
#include "../include/request.hpp"
#include "../include/cgi.hpp"
Response::Response()
{
}
Response::~Response()
{
}

void Response::handle_content_type()
{
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

std::string Response::get_message(int code)
{
    if (code == BAD_REQUEST)
        return "Bad Request";
    else if (code == METHOD_NOT_ALLOWED)
        return "Method Not Allowed";
    else if (code == NOT_FOUND)
        return "Not Found";
    else if (code == REQUEST_ENTITY_TOO_LARGE)
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
    else if (code == TEMPORARY_REDIRECT)
        return "Temporary Redirect";
    else if (code == UNAUTHORIZED)
        return "Unauthorized";
    else if (code == INTERNAL_SERVER_ERROR)
        return "Internal Server Error";
    else if (code == BAD_GATEWAY)
        return "Bad Gateway";
    else if (code == SERVICE_UNAVAILABLE)
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
std::string Response::get_file_extencion(std::string file)
{
    return file.substr(file.find_last_of(".") + 1);
}
std::string date()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %X %Z", &tstruct);
    return buf;
}

std::string Response::generate_response(Request *req)
{
    std::string response = "";
    response += req->version + " " + std::to_string(req->status_code) + " " + get_message(req->status_code) + "\r\n";
    response += "Server: webserv\r\n";
    response += "Date: " + date() + "\r\n";
    response += "Connection: " + req->headers.find("Connection")->second + "\r\n";
    if (req->redirect_path != "")
        response += "Location: " + req->redirect_path + "\r\n";
    if (this->body != "")
        response += "Content-Length: " + std::to_string(this->body.length()) + "\r\n";
    if (req->is_directory_file.first == true)
        response += "Content-Type: text/html\r\n";
    else if (req->is_directory_file.first == false && req->is_directory_file.second != "")
        response += "Content-Type: " + this->content_type[get_file_extencion(req->is_directory_file.second)] + "\r\n";
    response += "\r\n";

    if (req->redirect_path != "")
        response += req->redirect_path;
    else if (this->body != "")
        response += this->body;
    // response += "\r\n";
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
                res += "<a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a><br/>";
            }
        }
        closedir(dir);
    }
    res += "</pre><hr></body></html>";
    return res;
}

std::string Response::get_file(std::string path)
{
    std::ifstream file(path);
    std::string res;
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            res += line + "\n";
        }
        file.close();
    }
    return trim(res, "\n");
}

std::string Response::error_page(int status_code, std::map<int, std::string> error, std::string root)
{
    std::string res;
    if (error.find(status_code) != error.end())
    {
        if (file_exists(root + error[status_code]))
        {
            this->path = root + error[status_code];
            res = get_file(this->path);
        }
    }
    else
    {
        this->path = "";
        res = default_error_page(status_code);
    }
    return res;
}

std::string Response::default_delete_page()
{
    std::string res = "<html><head><title>Index of " + path + "</title></head><body bgcolor=\"white\"><h1>delete path</h1>>";
    res += "</pre><hr></body></html>";
    return res;
}

std::string Response::default_post_page(std::map<std::string, std::string> body_form_data)
{
    std::string res = "<html><head><title>Index of " + path + "</title></head><body bgcolor=\"white\"><h1>Index of " + path + "</h1><hr><pre>";
    std::map<std::string, std::string>::iterator it;
    for (it = body_form_data.begin(); it != body_form_data.end(); it++)
    {
        res += it->first + " : " + it->second + "\n";
    }
    res += "</pre><hr></body></html>";
    return res;
}
Response::Response(Request req)
{
    handle_content_type();
    if (req.redirect_path != "")
    {
        this->response_message = generate_response(&req);
        
        return;
    }
    if (req.status_code != OK)
    {
        this->body = error_page(req.status_code, req.error_page_map, req.root);
        this->response_message = generate_response(&req);
        return;
    }
    else
    {
        if (req.method == "DELETE")
        {
            this->body = default_delete_page();
            this->response_message = generate_response(&req);
            return;
        }
        else if (req.method == "POST")
        {
        
            if (req.is_directory_file.first == false && req.is_directory_file.second != "")
            {
                std::vector<std::string> vec_cgi;
                std::string extension = get_file_extencion(req.is_directory_file.second);
                vec_cgi = req._location.__attributes.find("cgi")->second;
                for(unsigned int i = 0; i < vec_cgi.size();i=i+2)
                {
                    if (vec_cgi[i] == extension)
                    {
                        std::string script = vec_cgi[i + 1];
                        std::string path = req.root + script;
                      
                        Cgi  _cgi(req, path);
                        this->body = _cgi.body;
                        // this->body = cgi(req, path);
                        // this->response_message = generate_response(&req);
                        // return;
                    }
                }

            }
            this->body = default_post_page(req.body_form_data);
            this->response_message = generate_response(&req);
            return;
        }
        else if (req.method == "GET")
        {
            if (req.is_directory_file.first)
            {
                if (req.is_autoindex)
                {
                    this->body = autoIndexPage(req.is_directory_file.second);
                    this->response_message = generate_response(&req);
                    return;
                }
                else
                {
                    this->body = error_page(403, req.error_page_map, req.root);
                    this->response_message = generate_response(&req);
                    return;
                }
            }
            else
            {
                std::string extension = get_file_extencion(req.is_directory_file.second);
                std::vector<std::string> vec_cgi;

                vec_cgi = req._location.__attributes.find("cgi")->second;
                for(unsigned int i = 0; i < vec_cgi.size();i=i+2)
                {
                    if (vec_cgi[i] == extension)
                    {
                        std::string script = vec_cgi[i+1];
                        std::string path = req.root + script;
                        Cgi _cgi(req, path);
                        this->body = _cgi.body;
                        // this->response_message = generate_response(&req);
                        // return;
                    }
                }
                this->body = get_file(req.is_directory_file.second);
                this->response_message = generate_response(&req);
                return;
            }
        }
    }
}


void Response::clear(){
    this->header.clear();
    this->body.clear();
    this->content_type.clear();
    this->path.clear();
}
