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

void Request::parseRequest(std::string req)
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

Request::Request(std::string req, Web *server)
{
	this->status_code = 0;
	this->parseRequest(req);
	if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
	{
		std::cout << "NOT_IMPLEMENTED" << std::endl;
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
		{
			std::cout << "different size body" << std::endl;
			this->status_code = BAD_REQUEST;
		}
	}
	this->checkServer(server);
	// if (this->_server.__attributes['autoindex'][0] == "on")
	//     this->is_autoindex = true;
	// else
	//     this->is_autoindex = false;
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
}

void Request::checkServer(Web *web)
{
	for (unsigned long i = 0; i < web->__servers.size(); i++)
	{
		std::map<std::string, std::vector<std::string> >::iterator it;
		for (it = web->__servers[i].__attributes.begin(); it != web->__servers[i].__attributes.end(); it++)
		{
			if (it->first == "listen")
			{
				std::vector<std::string> listen = it->second;
				for (unsigned long j = 0; j < listen.size(); j++)
				{
					std::vector<std::string> host_port = split(listen[j], ":");
					if (host_port[0] == this->host && host_port[1] == this->port)
					{
						std::cout << "server found server: " << host_port[0] << "port" << host_port[1] << std::endl;
						this->_server = web->__servers[i];
					}
				}
			}
		}
	}
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
