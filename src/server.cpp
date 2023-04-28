/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 17:26:56 by obelkhad          #+#    #+#             */
/*   Updated: 2023/04/28 18:19:35 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"
#include "../include/utils.hpp"

Location::Location()
{
	/*i feel like i need to put somthing here i don't know yet XD*/
}

Location::~Location()
{
	/*probably i'll need to free some containers*/
}

Server::Server() : 
__curly_location(false),
__curly_server(false),
__listen_default(true), 
__server_name_default(true)
{
	std::vector<std::string>	__default;

	/*LISTEN DEFAULT*/
	__default.push_back((std::string(ADDRESS).append(":")).append(PORT));
	__attributes["listen"] = __default;
	__default.clear();

	/*AUTOINDEX DEFAULT*/
	__default.push_back(std::string(AUTOINDEX));
	__attributes["autoindex"] = __default;
	__default.clear();

	/*INDEX DEFAULT*/
	__default.push_back(std::string(INDEX));
	__attributes["index"] = __default;
	__default.clear();

	/*CLIENT_BODY_MAX_SIZE DEFAULT*/
	__default.push_back(std::string(CLIENT_BODY_MAX_SIZE));
	__attributes["client_body_max_size"] = __default;
	__default.clear();

	/*SERVER NAME*/
	__default.push_back(std::string(SEVER_NAME));
	__attributes["server_name"] = __default;
	__default.clear();
	
	/*METHODS*/
	__default.push_back(std::string(METHODS));
	__attributes["methods"] = __default;
	__default.clear();
}

Server::~Server()
{}

Web::~Web()
{}

Web::Web()
{
	__handlers["server"] = &Web::__server;
	__handlers["listen"] = &Web::__listen;
	__handlers["location"] = &Web::__location;
	__handlers["server_name"] = &Web::__server_name;
	__handlers["cgi"] = &Web::__cgi;
	__handlers["root"] = &Web::__root;
	__handlers["index"] = &Web::__index;
	__handlers["redirect"] = &Web::__redirect;
	__handlers["autoindex"] = &Web::__autoindex;
	__handlers["error_page"] = &Web::__error_page;
	__handlers["upload_dir"] = &Web::__upload_dir;
	__handlers["methods"] = &Web::__methods;
	__handlers["client_body_max_size"] = &Web::__client_body_max_size;
	_l = 0;
	_s = 0;
}

/* ---------------------------------- PARSE --------------------------------- */
/* ---------------------------------- PARSE --------------------------------- */
/* ---------------------------------- PARSE --------------------------------- */
/* ---------------------------------- PARSE --------------------------------- */
void Web::__parse(std::string &__config_path)
{
	Location								__location;
	Server									__server;
	std::string 							__line;

	__file.open(__config_path);
	__file_fail(__file);
	while (!__file.eof() || !__line_splited.empty())
	{
		if (!__line_splited.empty())
		{
			if (__closed_bracket(__server, __location))
				continue;
			if (!__line_splited.empty())
			{
				if (__line_splited[0] == "server")
					__server = Server();
				if (__line_splited[0] == "location")
					__location = Location();
				(this->*__handlers.at(__line_splited[0]))(__server, __location);
			}
		}
		else
		{
			getline(__file, __line);
			__line = __extract_parameters(__line);
			__line_splited = __split_attrubites(__line, "{}; \t\r\v\f");
		}
	}
	__file.close();
	__set_locations();
}


/* --------------------------------- SERVER --------------------------------- */
/* --------------------------------- SERVER --------------------------------- */
/* --------------------------------- SERVER --------------------------------- */
/* --------------------------------- SERVER --------------------------------- */
void Web::__server(Server &__server, Location &__location)
{
	(void)__location;
	std::string __line;
	_s++;
	__server.__curly_server = true;
	if (__line_splited.size() == 1)
	{
		getline(__file, __line);
		__line = __extract_parameters(__line);
		__line_splited.clear();
		__line_splited = __split_attrubites(__line, "{}; \t\r\v\f");
		__curly_right_check(__line_splited[0]);
		__line_splited.erase(__line_splited.begin(), __line_splited.begin() + 1);
	}
	else if (__line_splited.size() > 1)
	{
		__curly_right_check(__line_splited[1]);
		__line_splited.erase(__line_splited.begin(), __line_splited.begin() + 2);
	}
}

/* -------------------------------- LOCATION -------------------------------- */
/* -------------------------------- LOCATION -------------------------------- */
/* -------------------------------- LOCATION -------------------------------- */
/* -------------------------------- LOCATION -------------------------------- */
void Web::__location(Server &__server, Location &__location)
{
	std::string __line;
	_l++;
	if (__line_splited[1] == "{")
	{
		std::cerr << "\033[1;31m" << "error: location missisng path!" << "\033[0m" << std::endl;
		exit(1);
	}
	if (__line_splited.size() == 2)
	{
		__location.__path = __line_splited[1];
		__line_splited.clear();
		getline(__file, __line);
		__line = __extract_parameters(__line);
		__line_splited = __split_attrubites(__line, "{}; \t\r\v\f");
		__curly_right_check(__line_splited[0]);
		__line_splited.erase(__line_splited.begin(), __line_splited.begin() + 1);
	}
	else if (__line_splited.size() > 2)
	{
		__location.__path = __line_splited[1];
		__curly_right_check(__line_splited[2]);
		__line_splited.erase(__line_splited.begin(), __line_splited.begin() + 3);
	}
	else
		__attributes_missing();
	__server.__curly_location = true;
}


void Web::__set_locations()
{
	std::vector<Server>::iterator							__it = __servers.begin();
	std::string												__h;
	std::pair<std::string, std::vector<std::string> >		__val;

	while (__it != __servers.end())
	{
		std::map<std::string, std::vector<std::string> >		*__m;		
		__m_iterator											__att;
		std::vector<Location>::iterator							__loc;

		__loc = __it->__locations.begin();
		while (__loc != __it->__locations.end())
		{
			__att = __it->__attributes.begin();
			__m = &(__loc->__attributes);
			while (__att != __it->__attributes.end())
			{
				__h = __att->first;

				if(__h != "listen" && __h != "server_name" && __m->find(__h) == __m->end())
				{
					__val.first = __h;
					__val.second = __att->second;
					__m->insert(__val);
				}
				++__att;	
			}
			++__loc;
		}
		++__it;
	}
}