#include "./request.hpp"

Request::Request()
{
    std::cout << "i am here" << std::endl;
}
Request::Request(std::string req)
{
    this->vec = split(req, "\r\n");
    std::vector<std::string> first_line = split(this->vec[0], " ");
    this->method = first_line[0];
    this->path = first_line[1];
    this->version = first_line[2];
    for (int i = 1; i < this->vec.size(); i++)
    {
        std::vector<std::string> header = split(this->vec[i], ":");
        if (header.size() == 2)
            this->headers[header[0]] = trim(header[1], " ");
    }
}
