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
		this->query_params = url_params[1];
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
	std::cout << this->query_params << std::endl;

	std::cout << "\n\n****************** body ******************" << std::endl;
	std::cout << this->body << std::endl;
}

Request::Request(Receive *__r)
{
	this->status_code = 0;
	this->is_directory_file.first = false;
	this->is_directory_file.second = "";
	this->is_autoindex = false;
	this->parseRequest(__r->__head, __r->__body);
	if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
	{
		this->status_code = NOT_IMPLEMENTED;
		return;
	}
	if (this->version != "HTTP/1.1" && this->version != "HTTP/1.0")
	{
		std::cout << "HTTP_VERSION_NOT_SUPPORTED" << std::endl;
		this->status_code = HTTP_VERSION_NOT_SUPPORTED;
		return;
	}
	if (this->body.size() > 0)
	{
		if (this->headers["Content-Length"] != std::to_string(this->body.size()))
			this->status_code = BAD_REQUEST;
			return;
	}
	if (this->body.size() > 0)
	{
		std::cout << this->_server.__attributes["client_body_max_size"][0] << std::endl;
		std::string body_size =trim(this->_server.__attributes["client_body_max_size"][0], " ");
		if(body_size[body_size.length() - 1] =='G')
		{
			if (this->body.size() > size_t(std::atoi(body_size.c_str()) * 1024 * 1024 * 1024))
			{
				this->status_code = REQUEST_ENTITY_TOO_LARGE;
				return;
			}
		}
		else if(body_size[body_size.length() - 1] =='M')
		{
			if (this->body.size() > size_t(std::atoi(body_size.c_str()) * 1024 * 1024))
			{
				this->status_code = REQUEST_ENTITY_TOO_LARGE;
				return;
			}
		}
		else if(body_size[body_size.length() - 1] =='B')
		{
			if (this->body.size() > size_t(std::atoi(body_size.c_str())))
				this->status_code = REQUEST_ENTITY_TOO_LARGE;
				return;
		}
		else
		{
			if (this->body.size() > size_t(std::atoi(body_size.c_str())))
				this->status_code = REQUEST_ENTITY_TOO_LARGE;
				return;
		}
	}
	this->_server = *(__r->__server);
	this->checkLocation();

	if (this->_location.__attributes["redirect"].size() > 0)
	{
		this->status_code = std::stoi(this->_location.__attributes["redirect"][0]);
		this->headers["Location"] = this->_location.__attributes["redirect"][1];
	}
	if (this->_location.__attributes["methods"].size() > 0)
	{
		for (unsigned long i = 0; i < this->_location.__attributes["methods"].size(); i++)
		{
			if (this->_location.__attributes["methods"][i] == this->method)
				break;
			else
			{
				this->status_code = METHOD_NOT_ALLOWED;
				return;
			}
		}
	}
	std::string path = "/";
	if (this->_location.__attributes["root"].size() > 0)
	{
		path += trim(this->_location.__attributes["root"][0], "/") + "/" + trim(this->path, "/");
		this->root = trim(this->_location.__attributes["root"][0], "/");
	}
	else
	{
		path += trim(this->_server.__attributes["root"][0], "/")  + "/" + trim(this->path, "/");
		this->root = trim(this->_server.__attributes["root"][0], "/");
	}
	if (this->method == "DELETE"){
		if (file_exists(path))
		{
			if (remove(path.c_str()) != 0)
			{
				this->status_code = INTERNAL_SERVER_ERROR;
				return;
			}
			else
			{
				this->status_code = OK;
				return;
			}
		}
		else
		{
			this->status_code = NOT_FOUND;
			return;
		}	
	}
	path = trim(path, "/");
	path = "/" + path;
	std::cout << "path: " << path << std::endl;
	if(is_directory(path))
	{
		path += "/";
		this->is_directory_file.first = true;
		this->is_directory_file.second = path;
		if(file_exists(path+"/index.html") && !is_directory(path+"/index.html"))
		{
			std::cout << "is file with index.html" << std::endl;
			this->is_directory_file.first = false;
			this->is_directory_file.second = path+"/index.html";
		}
		else
		{
			if (this->_location.__attributes["index"].size() > 0)
			{
				for (unsigned long i = 0; i < this->_location.__attributes["index"].size(); i++)
					{
						std::string index = this->_location.__attributes["index"][i];
						std::string index_path = this->is_directory_file.second + index;
						if (file_exists(index_path) && !is_directory(index_path))
						{
							std::cout << "is file with index" << std::endl;
							this->is_directory_file.first = false;
							this->is_directory_file.second = index_path;
							break;
						}
					}
			}
			else if (this->_server.__attributes["index"].size() > 0)
			{
				for (unsigned long i = 0; i < this->_server.__attributes["index"].size(); i++)
					{
						std::string index = this->_server.__attributes["index"][i];
						std::string index_path = this->is_directory_file.second + index;
						if (file_exists(index_path) && !is_directory(index_path))
						{
							std::cout << "is file with index" << std::endl;
							this->is_directory_file.first = false;
							this->is_directory_file.second = index_path;
							break;
						}
					}
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
			return;
		}
	}

	if (this->is_directory_file.first && this->_location.__attributes["autoindex"].size() > 0)
	{
		if (this->_location.__attributes["autoindex"][0] == "on")
			this->is_autoindex = true;
		else
		{
			this->status_code = NOT_FOUND;
			return;
		}
	}

	if(this->method == "POST"){
		std::string boundary ="--";
		boundary += this->headers["Content-Type"].substr(this->headers["Content-Type"].find("boundary=") + 9);
		this->body_boundary = split(this->body, boundary);

		for (unsigned long i = 0; i < this->body_boundary.size(); i++)
		{
			std::string name = this->body_boundary[i].substr(this->body_boundary[i].find("name=\"") + 6);
			name = name.substr(0, name.find("\""));
			std::string value = this->body_boundary[i].substr(this->body_boundary[i].find("\r\n\r\n") + 4);
			value = value.substr(0, value.find("\r\n"));

			if( this->body_boundary[i].find("filename=\"") != std::string::npos)
			{
				std::string filename = this->body_boundary[i].substr(this->body_boundary[i].find("filename=\"") + 10);
				filename = filename.substr(0, filename.find("\""));
				std::string file = this->_server.__attributes["upload_dir"][0]+"/"+filename;
				if(!file_exists(this->_server.__attributes["upload_dir"][0]))
					mkdir(this->_server.__attributes["upload_dir"][0].c_str(), 0777);
				std::ofstream out(file.c_str());
					out << value;
					out.close();
			}
			else
				this->body_form_data[name] = value;	
		}
	}

	if (this->status_code != 0){
		for (unsigned long i = 0; i < this->_location.__attributes["error_page"].size(); i=i+2)
		{
			this->error_page_map[std::atoi(this->_location.__attributes["error_page"][i].c_str())] = this->_location.__attributes["error_page"][i+1];
		}
	}
	else
		this->status_code = OK;
	
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
