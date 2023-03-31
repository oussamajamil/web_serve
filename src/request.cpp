/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oussama <oussama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:14:44 by oussama	          #+#    #+#            */
/*   Updated: 2023/03/24 21:45:22 by oussama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/request.hpp"
#include "../include/server.hpp"
Request::Request() {}

void Request::parseRequest(std::string header, std::string body)
{
	this->request = header + "\r\n\r\n" + body;
	this->body = body;

	std::vector<std::string> header_lines = split(header, "\r\n");
	std::vector<std::string> first_line = split(header_lines[0], " ");
	this->method = first_line[0];
	this->path = first_line[1];
	this->version = first_line[2];

	for (unsigned long i = 1; i < header_lines.size(); i++)
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
		for (unsigned long i = 0; i < params.size(); i++)
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

Request::Request(Receive *__r)
{
	this->status_code = 0;
	this->is_directory_file.first = false;
	this->is_directory_file.second = "";
	this->parseRequest(__r->__head, __r->__body);
	if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
	{
		this->status_code = NOT_IMPLEMENTED;
	}
	if (this->version != "HTTP/1.1" && this->version != "HTTP/1.0")
	{
		std::cout << "HTTP_VERSION_NOT_SUPPORTED" << std::endl;
		this->status_code = HTTP_VERSION_NOT_SUPPORTED;
	}

	if (this->method == "POST")
	{
		if (this->headers["Content-Length"] != std::to_string(this->body.size()))
			this->status_code = BAD_REQUEST;
	}
	this->_server = *(__r->__server);
	this->checkLocation();

	if (this->_location.__attributes["methods"].size() > 0)
	{
		for (unsigned long i = 0; i < this->_location.__attributes["methods"].size(); i++)
		{
			if (this->_location.__attributes["methods"][i] == this->method)
				break;
			else
				this->status_code = METHOD_NOT_ALLOWED;
		}
	}
	std::string path = "/";
	if (this->_location.__attributes["root"].size() > 0)
	{
		path += trim(this->_location.__attributes["root"][0], "/") + "/" + trim(this->path, "/");
	}
	else
		path += trim(this->_server.__attributes["root"][0], "/")  + "/" + trim(this->path, "/");
	
	path = trim(path, "/");
	path = "/" + path;
	if(is_directory(path))
	{
		path += "/";
		this->is_directory_file.first = true;
		this->is_directory_file.second = path;
		for (unsigned long i = 0; i < this->_location.__attributes["index"].size(); i++)
		{
			std::string index = this->_location.__attributes["index"][i];
			std::string index_path = this->is_directory_file.second + index;
			if (file_exists(index_path))
			{
				this->is_directory_file.first = false;
				this->is_directory_file.second = index_path;
				break;
			}
		}
	}
	else
	{
		if(file_exists(path)){
			this->is_directory_file.first = false;
			this->is_directory_file.second =path;
		}
		else
		{
			this->status_code = NOT_FOUND;
		}
	}

	
	// std::cout << "path: " << path << std::endl;
	// std::cout << "path_content: " << this->path_content << std::endl;
	// if (!this->is_fileexist)
	// {
	// 	std::cout << "file doesnt exists" << std::endl;
	// 	this->status_code = NOT_FOUND;
	// }
	// else
	// {
	// 	std::cout << "file exists" << std::endl;
	// 	if (is_directory(this->path_content))
	// 	{

	// 		std::cout << "is directory" << std::endl;
	// 		this->is_directory_file.first = true;
	// 		this->is_directory_file.second = this->path_content;
	// 	}
	// 	else
	// 	{
	// 		std::cout << "is file" << std::endl;	
	// 		this->is_directory_file.first = false;
	// 		this->is_directory_file.second = this->path_content;
	// 	}
	// }


}

void Request::checkLocation()
{
	unsigned long match = 0;
	int indexLocation = -1;
	std::vector<Location> locations = this->_server.__locations;
	for (unsigned long i = 0; i < locations.size(); i++)
	{
		if (locations[i].__path == this->path)
		{
			this->_location = locations[i];
			return;
		}
		else if (this->path.find(locations[i].__path) == 0)
		{
			if (match < locations[i].__path.size())
			{
				match = locations[i].__path.size();
				indexLocation = i;
			}
		}
	}
	if (indexLocation != -1)
	{
		this->_location = locations[indexLocation];
	}
}

Request::~Request() {}
