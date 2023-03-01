/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 17:26:56 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/01 16:30:54 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "../include/server.hpp"
#include "../include/utils.hpp"
#include "../include/errors.hpp"
#include "../include/default.hpp"

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
__index_default(true), 
__listen_default(true), 
__autoindex_default(true),
__client_body_max_size_default(true)
{
	/*i feel like i need to put somthing here i don't know yet XD*/
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

	/*ROOT DEFAULT*/
	__default.push_back(std::string(ROOT));
	__attributes["root"] = __default;
	__default.clear();

	/*CLIENT_BODY_MAX_SIZE DEFAULT*/
	__default.push_back(std::string(CLIENT_BODY_MAX_SIZE));
	__attributes["client_body_max_size"] = __default;
	__default.clear();
}

Server::~Server()
{
	/*probably i'll need to free some containers*/
}

Web::~Web()
{
	/*probably i'll need to free some containers*/
}

Web::Web() : __curly_server(false)
{
	/*i feel like i need to put somthing here i don't know yet XD*/
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
}

void Web::__initial_action(Server &__server)
{
	/*check syntax*/
	if (!__curly_server)
		__bad_syntax();


	/*check if attribute in a wrong zone*/
	if (__server.__curly_location)
		__wrong_arg();


	/* erase "atributte key word"*/
	__line_splited.erase(__line_splited.begin());
	

	/* handling errors*/
	if (std::find(__line_splited.begin(), __line_splited.end(), ";") == __line_splited.end())
		__semi_colon_missing();
	if (__line_splited.size() == 1)
		__attributes_missing();
}

std::vector<Server> Web::__get_servers()
{ return __servers; }


/* ---------------------------------- PARSE --------------------------------- */
/* ---------------------------------- PARSE --------------------------------- */
/* ---------------------------------- PARSE --------------------------------- */
/* ---------------------------------- PARSE --------------------------------- */
void Web::__parse(std::string &__config_path)
{
	Server									__server;
	std::string 							__line;

	__file.open(__config_path);
	__file_fail(__file);
	while (!__file.eof() || !__line_splited.empty())
	{
		if (!__line_splited.empty())
		{
			if (__line_splited[0] == "}")
			{
				if (__server.__curly_location && __curly_server)
					__server.__curly_location = false;
				else if (__curly_server)
				{
					__curly_server = false;
					__servers.push_back(__server);
				}
				__line_splited.erase(__line_splited.begin(), __line_splited.begin() + 1);
				continue;
			}
			std::cout << "[ " << __curly_server << " ]" << '\n';
			std::cout << "[ " << __server.__curly_location << " ]" << '\n';
			__vector_display(__line_splited);
			(this->*__handlers.at(__line_splited[0]))(__server);
			continue;
		}
		else
		{
			getline(__file, __line);
			__line = __extract_parameters(__line);
			__line_splited = __split_attrubites(__line, "{}; \t\r\v\f");
		}
	}
	__file.close();
}


/* --------------------------------- SERVER --------------------------------- */
/* --------------------------------- SERVER --------------------------------- */
/* --------------------------------- SERVER --------------------------------- */
/* --------------------------------- SERVER --------------------------------- */
void Web::__server(Server &__server)
{
	(void)__server;
	std::string __line;

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
	__curly_server = true;
}

/* --------------------------------- LISTEN --------------------------------- */
/* --------------------------------- LISTEN --------------------------------- */
/* --------------------------------- LISTEN --------------------------------- */
/* --------------------------------- LISTEN --------------------------------- */
void Web::__listen(Server &__server)
{
	__m_iterator				__listen = __server.__attributes.find("listen");
	std::vector<std::string>	__values;

	/*remove default configuration*/
	if (__server.__listen_default)
	{
		__listen->second.clear();
		__server.__listen_default = false;
	}
	
	/*initial action*/
	__initial_action(__server);


	/*parse listen args*/
	__listen = __server.__attributes.find("listen");
	__values = __parse_listen_args();
	__listen->second.insert(__listen->second.end(), __values.begin(), __values.end());
}

std::vector<std::string> Web::__parse_listen_args()
{
	std::vector<std::string>	__holder;
	
	while (__line_splited.front() != ";")	
	{
		//TODO:check form of IP
		//TODO:check range of IP
		//TODO:check range of PORT
		__line_splited.erase(__line_splited.begin());
		//TODO:add default PORT to host thar come without PORT
	}

	/* remove ";" */
	__line_splited.erase(__line_splited.begin());

	return __holder;
}


/* -------------------------------- LOCATION -------------------------------- */
/* -------------------------------- LOCATION -------------------------------- */
/* -------------------------------- LOCATION -------------------------------- */
/* -------------------------------- LOCATION -------------------------------- */
void Web::__location(Server &__server)
{
	std::string __line;

	if (__line_splited.size() == 2)
	{
		getline(__file, __line);
		__line = __extract_parameters(__line);
		__line_splited.clear();
		__line_splited = __split_attrubites(__line, "{}; \t\r\v\f");
		//TODO: handline "{"
		__curly_right_check(__line_splited[0]);
		__line_splited.erase(__line_splited.begin(), __line_splited.begin() + 1);
	}
	else if (__line_splited.size() > 2)
	{
		__curly_right_check(__line_splited[2]);
		__line_splited.erase(__line_splited.begin(), __line_splited.begin() + 3);
	}
	__server.__curly_location = true;
}

/* ------------------------------- SERVER NAME ------------------------------ */
/* ------------------------------- SERVER NAME ------------------------------ */
/* ------------------------------- SERVER NAME ------------------------------ */
/* ------------------------------- SERVER NAME ------------------------------ */
void Web::__server_name(Server &__server)
{
	/*initial action*/
	__initial_action(__server);

	/*add valuses*/
	while (__line_splited.front() != ";")	
	{
		(__server.__attributes["server_name"]).push_back(__line_splited.front());
		__line_splited.erase(__line_splited.begin());
	}
	__line_splited.erase(__line_splited.begin());
}

/* ----------------------------------- CGI ---------------------------------- */
/* ----------------------------------- CGI ---------------------------------- */
/* ----------------------------------- CGI ---------------------------------- */
/* ----------------------------------- CGI ---------------------------------- */
void Web::__cgi(Server &__server)
{
	/*initial action*/
	__initial_action(__server);

	/*add valuses*/
	while (__line_splited.front() != ";")	
	{
		(__server.__attributes["cgi"]).push_back(__line_splited.front());
		__line_splited.erase(__line_splited.begin());
	}
	__line_splited.erase(__line_splited.begin());
}

/* ---------------------------------- ROOT ---------------------------------- */
/* ---------------------------------- ROOT ---------------------------------- */
/* ---------------------------------- ROOT ---------------------------------- */
/* ---------------------------------- ROOT ---------------------------------- */
void Web::__root(Server &__server)
{
	/*initial action*/
	__initial_action(__server);

	/*add valuses*/
	while (__line_splited.front() != ";")	
	{
		(__server.__attributes["cgi"]).push_back(__line_splited.front());
		__line_splited.erase(__line_splited.begin());
	}
	__line_splited.erase(__line_splited.begin());
	if (__server.__attributes["cgi"].size() != 1)
		__bad_number_arguments();
}

/* ---------------------------------- INDEX --------------------------------- */
/* ---------------------------------- INDEX --------------------------------- */
/* ---------------------------------- INDEX --------------------------------- */
/* ---------------------------------- INDEX --------------------------------- */
void Web::__index(Server &__server)
{	(void)__server;
	// __vector_display(__line_splited);
	__line_splited.clear();
}

/* ------------------------------- REDIRECTION ------------------------------ */
/* ------------------------------- REDIRECTION ------------------------------ */
/* ------------------------------- REDIRECTION ------------------------------ */
/* ------------------------------- REDIRECTION ------------------------------ */
void Web::__redirect(Server &__server)
{	(void)__server;
	// __vector_display(__line_splited);
	__line_splited.clear();
}

/* -------------------------------- AUTOINDEX ------------------------------- */
/* -------------------------------- AUTOINDEX ------------------------------- */
/* -------------------------------- AUTOINDEX ------------------------------- */
/* -------------------------------- AUTOINDEX ------------------------------- */
void Web::__autoindex(Server &__server)
{	(void)__server;
	// __vector_display(__line_splited);
	__line_splited.clear();
}

/* ------------------------------- ERROR PAGES ------------------------------ */
/* ------------------------------- ERROR PAGES ------------------------------ */
/* ------------------------------- ERROR PAGES ------------------------------ */
/* ------------------------------- ERROR PAGES ------------------------------ */
void Web::__error_page(Server &__server)
{	(void)__server;
	// __vector_display(__line_splited);
	__line_splited.clear();
}

/* ------------------------------- UPLOAD DIR ------------------------------- */
/* ------------------------------- UPLOAD DIR ------------------------------- */
/* ------------------------------- UPLOAD DIR ------------------------------- */
/* ------------------------------- UPLOAD DIR ------------------------------- */
void Web::__upload_dir(Server &__server)
{	(void)__server;
	// __vector_display(__line_splited);
	__line_splited.clear();
}

/* --------------------------------- METHODS -------------------------------- */
/* --------------------------------- METHODS -------------------------------- */
/* --------------------------------- METHODS -------------------------------- */
/* --------------------------------- METHODS -------------------------------- */
void Web::__methods(Server &__server)
{	(void)__server;
	// __vector_display(__line_splited);
	__line_splited.clear();
}

/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
void Web::__client_body_max_size(Server &__server)
{	(void)__server;
	// __vector_display(__line_splited);
	__line_splited.clear();
}