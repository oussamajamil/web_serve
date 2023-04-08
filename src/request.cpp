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
	// std::cout << "****************** request ******************" << std::endl;
	// std::cout << "\033[1;32m request: \033[0m" << this->request << std::endl;
	// std::cout << "****************** body ******************" << std::endl;
	// std::cout << "\033[1;32m body: \033[0m" << this->body << std::endl;
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

	// std::cout << "****************** path/version/method ******************" << std::endl;
	// std::cout << "\033[1;32m method:  \033[0m"
	// 					<< this->method << std::endl;
	// std::cout << "\033[1;32m path:    \033[0m"
	// 					<< this->path << std::endl;
	// std::cout << "\033[1;32m version: \033[0m"
	// 					<< this->version << std::endl;
	// std::cout << "\033[1;32m host:    \033[0m"
	// 					<< this->host << std::endl;
	// std::cout << "\033[1;32m port:    \033[0m"
	// 					<< this->port << std::endl;
	// std::cout << "\n\n****************** headers ******************" << std::endl;
	// std::map<std::string, std::string>::iterator it;
	// for (it = this->headers.begin(); it != this->headers.end(); it++)
	// {
	// 	std::cout << "\033[1;32m " << it->first << ": \033[0m" << it->second << std::endl;
	// 	std::cout << std::endl;
	// }

	// std::cout << "\n\n****************** query_params ******************" << std::endl;
	// std::cout << this->query_params << std::endl;

	// std::cout << "\n\n****************** body ******************" << std::endl;
	// std::cout << this->body << std::endl;
}

Request::Request(Receive *__r)
{
	this->status_code = 0;
	this->path_res = "/";
	this->is_directory_file.first = false;
	this->is_directory_file.second = "";
	this->is_autoindex = false;
	this->redirect_path = "";
	this->parseRequest(__r->__head, __r->__body);
	this->_connection = this->headers["Connection"];
	if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
	{
		this->status_code = NOT_IMPLEMENTED;
		return;
	}
	if (this->version != "HTTP/1.1" && this->version != "HTTP/1.0")
	{
		this->status_code = HTTP_VERSION_NOT_SUPPORTED;
		return;
	}
	if (this->body.size() > 0)
	{
		if (this->headers["Content-Length"] != std::to_string(this->body.size()))
		{
			this->status_code = BAD_REQUEST;
			return;
		}
	}
	if (this->body.size() > 0)
	{

		std::string body_size = trim(this->_server.__attributes["client_body_max_size"][0], " ");
		if (body_size[body_size.length() - 1] == 'G')
		{
			if (this->body.size() > size_t(std::atoi(body_size.c_str()) * 1024 * 1024 * 1024))
			{
				this->status_code = REQUEST_ENTITY_TOO_LARGE;
				return;
			}
		}
		else if (body_size[body_size.length() - 1] == 'M')
		{
			if (this->body.size() > size_t(std::atoi(body_size.c_str()) * 1024 * 1024))
			{
				this->status_code = REQUEST_ENTITY_TOO_LARGE;
				return;
			}
		}
		else if (body_size[body_size.length() - 1] == 'B')
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
		body_size = "";
	}
	this->_server = *(__r->__server);
	this->checkLocation();

	if (this->_location.__attributes["redirect"].size() > 0)
	{
		this->status_code = std::stoi(this->_location.__attributes["redirect"][0]);
		this->redirect_path = this->_location.__attributes["redirect"][1];
		return;
	}
	if (this->_location.__attributes["methods"].size() > 0)
	{
		unsigned long i;
		for (i = 0; i < this->_location.__attributes["methods"].size(); i++)
		{
			if (this->_location.__attributes["methods"][i] == this->method)
				break;
		}
		if (i == this->_location.__attributes["methods"].size())
		{
			this->status_code = METHOD_NOT_ALLOWED;
			return;
		}
	}
	if (this->_location.__attributes["root"].size() > 0)
	{
		this->path_res += trim(this->_location.__attributes["root"][0], "/") + "/" + trim(this->path, "/");
		this->root = trim(this->_location.__attributes["root"][0], "/");
	}
	else
	{
		this->path_res += trim(this->_server.__attributes["root"][0], "/") + "/" + trim(this->path, "/");
		this->root = trim(this->_server.__attributes["root"][0], "/");
	}
	if (this->method == "DELETE")
	{

		if (file_exists(this->path_res))
		{
			
			if (remove(this->path_res.c_str()) != 0)
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
	this->path_res = trim(this->path_res, "/");
	this->path_res = "/" + this->path_res;

	if (is_directory(this->path_res))
	{
		this->path_res += "/";
		this->is_directory_file.first = true;
		this->is_directory_file.second = this->path_res;
		if (file_exists(this->path_res + "/index.html") && !is_directory(this->path_res + "/index.html"))
		{
			
			this->is_directory_file.first = false;
			this->is_directory_file.second = this->path_res + "/index.html";
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
						this->is_directory_file.first = false;
						this->is_directory_file.second = index_path;
						break;
					}
				}
			}
			else
			{
					this->status_code = NOT_FOUND;
					return;
			}
		}
		
	}
	else
	{
		if (file_exists(this->path_res))
		{
			this->is_directory_file.first = false;
			this->is_directory_file.second = this->path_res;
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

	if (this->method == "POST")
	{
		std::string boundary = "--";
	
		boundary += this->headers["Content-Type"].substr(this->headers["Content-Type"].find("boundary=") + 9);
		this->body_boundary = split(this->body, boundary);

		for (unsigned long i = 0; i < this->body_boundary.size()-1; i++)
		{
			std::string name = this->body_boundary[i].substr(this->body_boundary[i].find("name=\"") + 6);
			name = name.substr(0, name.find("\""));
			std::string value = this->body_boundary[i].substr(this->body_boundary[i].find("\r\n\r\n") + 4);
			
			if (this->body_boundary[i].find("filename=\"") != std::string::npos)
			{
				value = value.substr(0, value.find_last_of("\r\n"));
				std::string filename = this->body_boundary[i].substr(this->body_boundary[i].find("filename=\"") + 10);
				filename = filename.substr(0, filename.find("\""));
				std::string file = this->_server.__attributes["upload_dir"][0] + "/" + filename;
				if (!file_exists(this->_server.__attributes["upload_dir"][0]))
					mkdir(this->_server.__attributes["upload_dir"][0].c_str(), 0777);
				std::ofstream out(file.c_str());
				
				out << value;
				out.close();
			}
			else
			{
				value = value.substr(0, value.find("\r\n"));
				this->body_form_data[name] = value;
			}
		}
	}

	if (this->status_code != 0)
	{
		for (unsigned long i = 0; i < this->_location.__attributes["error_page"].size(); i = i + 2)
		{
			this->error_page_map[std::atoi(this->_location.__attributes["error_page"][i].c_str())] = this->_location.__attributes["error_page"][i + 1];
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
void Request::clear()
{
	this->method.clear();
	this->path.clear();
	this->version.clear();
	this->headers.clear();
	this->query_params.clear();
	this->body.clear();
	this->port.clear();
	this->is_autoindex = false;
	this->status_code = 0;
	this->body_boundary.clear();
	this->body_form_data.clear();
	this->error_page_map.clear();
	this->is_directory_file.first = false;
	this->is_directory_file.second="";
	this->redirect_path = "";
	this->root = "";
	this->_connection = "";
	this->request = "";
	this->path_res.clear();
}
Request::~Request() {}
