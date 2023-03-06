/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 18:29:55 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/04 16:03:08 by obelkhad         ###   ########.fr       */
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
public:
	std::string												__path;
	std::map<std::string, std::vector<std::string> >		__attributes;
	Location();
	~Location();
};

class Server
{
public:
	std::vector<Location>									__locations;
	std::map<std::string, std::vector<std::string> >		__attributes;

	/*Curly bracket flags*/
	bool													__curly_location;
	bool													__curly_server;

	/*Default configuration flags*/
	bool													__listen_default;
	bool													__autoindex_default;
	bool													__index_default;
	bool													__root_default;
	bool													__client_body_max_size_default;
	bool													__server_name_default;
	bool													__methods_default;
	bool													__upload_dir_default;
	bool													__error_page_default;
	bool													__cgi_default;
	bool													__redirect_default;
	
	Server();
	~Server();
	
};

class Web
{
private:
	typedef void (Web::*handler)(Server&, Location&);
	std::map<std::string, handler>												__handlers;
	typedef std::vector<std::string>::iterator									__v_iterator;
	typedef std::map<std::string, std::vector<std::string> >::iterator			__m_iterator;
	std::vector<std::string> 													__line_splited;
	std::ifstream																__file;


	void __server(Server &__server, Location &__location);
	void __listen(Server &__server, Location &__location);
	void __location(Server &__server, Location &__location);
	void __server_name(Server &__server, Location &__location);
	void __cgi(Server &__server, Location &__location);
	void __root(Server &__server, Location &__location);
	void __index(Server &__server, Location &__location);
	void __redirect(Server &__server, Location &__location);
	void __autoindex(Server &__server, Location &__location);
	void __error_page(Server &__server, Location &__location);
	void __upload_dir(Server &__server, Location &__location);
	void __methods(Server &__server, Location &__location);
	void __client_body_max_size(Server &__server, Location &__location);


	
public:
	Web();
	~Web();

	std::vector<Server>	 		__servers;										//array of servers
	void						__parse(std::string &__config_path);
	// void						__server_up();

	std::vector<std::string>	__parse_listen_args();
	std::vector<std::string>	__parse_args();
	void						__initial_action(Server &__server);
	bool 						__closed_bracket(Server &__server, Location &__location);
};

 