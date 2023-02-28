#include "./request.hpp"

Request::Request() {}
Request::Request(std::string req)
{
    this->request = req;
    std::vector<std::string> lines = split(req, "\r\n\r\n");
    std::string header = lines[0];
    this->body = lines[1];

    std::vector<std::string> header_lines = split(header, "\r\n");
    std::vector<std::string> first_line = split(header_lines[0], " ");
    this->method = first_line[0];
    this->path = first_line[1];
    this->version = first_line[2];

    for (int i = 1; i < header_lines.size(); i++)
    {
        std::vector<std::string> header_line = split(header_lines[i], ": ");
        if (header_line[0] == "Host")
        {
            std::vector<std::string> host_port = split(header_line[1], ":");
            this->host = host_port[0];
            this->port = host_port[1];
        }
        this->headers[header_line[0]] = header_line[1];
    }
    std::vector<std::string> url_params = split(this->path, "?");
    if (url_params.size() > 1)
    {
        this->path = url_params[0];
        std::vector<std::string> params = split(url_params[1], "&");
        for (int i = 0; i < params.size(); i++)
        {
            std::vector<std::string> param = split(params[i], "=");
            this->query_params[param[0]] = param[1];
        }
    }
    std::cout << "****************** path/version/method ******************" << std::endl;
    std::cout << "\033[1;32m method:  \033[0m"
              << this->method << std::endl;
    std::cout << "\033[1;32m path:    \033[0m"
              << this->path << std::endl;
    std::cout << "\033[1;32m version: \033[0m"
              << this->version << std::endl;
    std::cout << "\033[1;32m host:    \033[0m"
              << this->host << std::endl;
    std::cout << "\033[1;32m port:    \033[0m"
              << this->port << std::endl;
    std::cout << "\n\n****************** headers ******************" << std::endl;
    std::map<std::string, std::string>::iterator it;
    for (it = this->headers.begin(); it != this->headers.end(); it++)
    {
        std::cout << "\033[1;32m " << it->first << ": \033[0m" << it->second << std::endl;
        std::cout << std::endl;
    }

    std::cout << "\n\n****************** query_params ******************" << std::endl;
    for (it = this->query_params.begin(); it != this->query_params.end(); it++)
    {
        std::cout << "\033[1;32m " << it->first << ": \033[0m" << it->second << std::endl;
        std::cout << std::endl;
    }

    std::cout << "\n\n****************** body ******************" << std::endl;
    std::cout << this->body << std::endl;
}
