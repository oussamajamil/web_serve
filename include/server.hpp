/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 18:29:55 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/01 13:49:29 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class Location
{
private:
	std::map<std::string, std::vector<std::string> >		__attributes;
public:
	Location();
	~Location();
};

class Server
{
public:
	std::vector<Location>									__locations;
	std::map<std::string, std::vector<std::string> >		__attributes;

	bool													__curly_location;

	bool													__index_default;
	bool													__listen_default;
	bool													__autoindex_default;
	bool													__client_body_max_size_default;
	
	Server();
	~Server();
	
};

class Web
{
private:
	typedef void (Web::*handler)(Server&);
	std::map<std::string, handler>												__handlers;
	typedef std::vector<std::string>::iterator									__v_iterator;
	typedef std::map<std::string, std::vector<std::string> >::iterator			__m_iterator;
	std::vector<Server>	 														__servers;			//array of servers
	std::vector<std::string> 													__line_splited;
	std::ifstream																__file;
	bool																		__curly_server;


	void __server(Server &__server);
	void __listen(Server &__server);
	void __location(Server &__server);
	void __server_name(Server &__server);
	void __cgi(Server &__server);
	void __root(Server &__server);
	void __index(Server &__server);
	void __redirect(Server &__server);
	void __autoindex(Server &__server);
	void __error_page(Server &__server);
	void __upload_dir(Server &__server);
	void __methods(Server &__server);
	void __client_body_max_size(Server &__server);


	
public:
	Web();
	std::vector<Server>			__get_servers();
	void						__parse(std::string &__config_path);
	std::vector<std::string>	__parse_listen_args();
	void						__initial_action(Server &__server);

	~Web();
};

 