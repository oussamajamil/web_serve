/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utl_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 16:12:30 by obelkhad          #+#    #+#             */
/*   Updated: 2023/04/27 18:03:39 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/server.hpp"
#include "../include/utils.hpp"
#include "../utils.hpp"

void Web::__initial_action(Server &__server)
{
	/*check syntax*/
	if (!__server.__curly_server)
		__bad_syntax();

	/* erase "atributte key word"*/
	__line_splited.erase(__line_splited.begin());	

	/* handling errors*/
	if (std::find(__line_splited.begin(), __line_splited.end(), ";") == __line_splited.end())
		__semi_colon_missing();
	if (__line_splited.size() == 1)
		__attributes_missing();
}

bool Web::__closed_bracket(Server &__server, Location &__location)
{
	if (__line_splited[0] == "}")
	{
		if (__server.__curly_location)
		{
			__server.__curly_location = false;
			__server.__locations.push_back(__location);
		}
		else if (__server.__curly_server)
		{
			if (__server.__locations.size() == 0)
			{
				std::cerr << "\033[1;31m" << "error: missing location" << "\033[0m" << std::endl;
				exit(1);
			}
			if (__server.__attributes.find("root") == __server.__attributes.end())
			{
				std::cerr << "\033[1;31m" << "root missing" << "\033[0m" << std::endl;
				exit(1);

			}
			__server.__curly_server = false;
			__servers.push_back(__server);
		}
		__line_splited.erase(__line_splited.begin(), __line_splited.begin() + 1);
		return true;
	}
	return false;
}

std::vector<std::string> Web::__parse_args()
{
	std::vector<std::string>	__holder;
	
	while (__line_splited.front() != ";")
	{
		__holder.push_back(__line_splited.front());
		__line_splited.erase(__line_splited.begin());
	}

	/* remove ";" */
	__line_splited.erase(__line_splited.begin());
	return __holder;
}

std::vector<std::string> Web::__parse_listen_args()
{
	std::vector<std::string>	__holder;
	
	while (__line_splited.front() != ";")
	{
		/* CHECK IP FORMT*/
		__address_prot_form(__line_splited.front());
	
		__holder.push_back(__line_splited.front());
		
		__line_splited.erase(__line_splited.begin());
	}
	/* remove ";" */
	__line_splited.erase(__line_splited.begin());

	return __holder;
}

/* --------------------------------- LISTEN --------------------------------- */
/* --------------------------------- LISTEN --------------------------------- */
/* --------------------------------- LISTEN --------------------------------- */
/* --------------------------------- LISTEN --------------------------------- */
void Web::__listen(Server &__server, Location &__location)
{
	__m_iterator				listen = __server.__attributes.find("listen");
	std::vector<std::string>	__values;
	(void)__location;

	/*CHECK ZONE*/
	if (__server.__curly_location)
		__wrong_location_attribute();

	/*initial action*/
	__initial_action(__server);

	// /*remove default configuration*/
	if (__server.__listen_default)
	{
		listen->second.clear();
		__server.__listen_default = false;
	}

	/*parse listen args*/
	__values = __parse_listen_args();
	listen->second.insert(listen->second.end(), __values.begin(), __values.end());
}

/* ------------------------------- SERVER NAME ------------------------------ */
/* ------------------------------- SERVER NAME ------------------------------ */
/* ------------------------------- SERVER NAME ------------------------------ */
/* ------------------------------- SERVER NAME ------------------------------ */
void Web::__server_name(Server &__server, Location &__location)
{
	__m_iterator				server_name = __server.__attributes.find("server_name");
	std::vector<std::string>	__values;
	(void)__location;

	/*CHECK ZONE*/
	if (__server.__curly_location)
		__wrong_location_attribute();

	/*initial action*/
	__initial_action(__server);

	// /*remove default configuration*/
	if (__server.__server_name_default)
	{
		server_name->second.clear();
		__server.__server_name_default = false;
	}

	/*add valuses*/
	__values = __parse_args();
	server_name->second.insert(server_name->second.end(), __values.begin(), __values.end());
}

/* --------------------------------- METHODS -------------------------------- */
/* --------------------------------- METHODS -------------------------------- */
/* --------------------------------- METHODS -------------------------------- */
/* --------------------------------- METHODS -------------------------------- */
void Web::__methods(Server &__server, Location &__location)
{
	static	int					__l;
	static	int					__s;
	static	int					__lsize;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);
	
	/*extract values*/
	__values = __parse_args();
	for (size_t __i = 0; __i < __values.size(); __i++)
	{
		if(__values[__i] != "DELETE" && __values[__i] != "GET" && __values[__i] != "POST")
		{
			std::cerr << "\033[1;31m" << "error: wrong autoindex attribute" << "\033[0m" << std::endl;
			exit(1);
		}
	}
	/*CHECK LOCATION*/
	if (__server.__curly_location)
	{
		if (__lsize < __server.__l)
			__l = 0;
		__lsize = __server.__l;
		if(__values.size() > 3 || __l)
		{
			std::cerr << "\033[1;31m" << "error: method attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__l = 1;
		__location.__attributes["methods"] = __values;
	}
	else
	{
		if(__values.size() > 3 || __s)
		{
			std::cerr << "\033[1;31m" << "error: method attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__s = 1;
		/*add valuses*/
		__server.__attributes["methods"] = __values;
	}
}
/* ----------------------------------- CGI ---------------------------------- */
/* ----------------------------------- CGI ---------------------------------- */
/* ----------------------------------- CGI ---------------------------------- */
/* ----------------------------------- CGI ---------------------------------- */
void Web::__cgi(Server &__server, Location &__location)
{
	__m_iterator				cgi;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);
	
	/*extract values*/
	__values = __parse_args();
	
	if (__values.size() != 2)
	{
		std::cerr << "\033[1;31m" << "Error : cgi attribute" << "\033[0m" << std::endl;
		exit(1);	
	}
	if (!file_exists(__values[1]))
	{
		std::cerr << "\033[1;31m" << "\" " << __values[1] << " \" : file doesn't exist" << "\033[0m" << std::endl;
		exit(1);
	}
	/*CHECK LOCATION*/
	if (__server.__curly_location)
		__location.__attributes["cgi"].insert(__location.__attributes["cgi"].end(), __values.begin(), __values.end());
	else
	{
		if (__server.__attributes.find("cgi") == __server.__attributes.end())
			__server.__attributes["cgi"];
		cgi = __server.__attributes.find("cgi");
		cgi->second.insert(cgi->second.end(), __values.begin(), __values.end());
	}
}

/* ---------------------------------- ROOT ---------------------------------- */
/* ---------------------------------- ROOT ---------------------------------- */
/* ---------------------------------- ROOT ---------------------------------- */
/* ---------------------------------- ROOT ---------------------------------- */
void Web::__root(Server &__server, Location &__location)
{
	static	int					__l;
	static	int					__s;
	static	int					__lsize;
	__m_iterator				root;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
	{
		if (__lsize < __server.__l)
			__l = 0;
		__lsize = __server.__l;
		if(__values.size() > 1 || __l)
		{
			std::cerr << "\033[1;31m" << "error: root attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__l = 1;
		__location.__attributes["root"] = __values;
	}
	else
	{
		if(__values.size() > 1 || __s)
		{
			std::cerr << "\033[1;31m" << "error: root attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__s = 1;
		__server.__attributes["root"] = __values;

	
	}
}

/* ---------------------------------- INDEX --------------------------------- */
/* ---------------------------------- INDEX --------------------------------- */
/* ---------------------------------- INDEX --------------------------------- */
/* ---------------------------------- INDEX --------------------------------- */
void Web::__index(Server &__server, Location &__location)
{
	static	int					__l;
	static	int					__s;
	static	int					__lsize;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
	{
		if (__lsize < __server.__l)
			__l = 0;
		__lsize = __server.__l;
		if(__values.size() > 1 || __l)
		{
			std::cerr << "\033[1;31m" << "error: index attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__l = 1;
		__location.__attributes["index"] = __values;
	}
	else
	{
		if(__values.size() > 1 || __s)
		{
			std::cerr << "\033[1;31m" << "error: index attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__s = 1;

		/*add valuses*/
		__server.__attributes["index"] = __values;
	}
}

/* ------------------------------- REDIRECTION ------------------------------ */
/* ------------------------------- REDIRECTION ------------------------------ */
/* ------------------------------- REDIRECTION ------------------------------ */
/* ------------------------------- REDIRECTION ------------------------------ */
void Web::__redirect(Server &__server, Location &__location)
{
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);


	/*extract values*/
	__values = __parse_args();


	/*check syntax*/
	if (!__server.__curly_location)
		__bad_syntax();

	/*add valuses*/
	__location.__attributes["redirect"].insert(__location.__attributes["redirect"].end(), __values.begin(), __values.end());
}

/* -------------------------------- AUTOINDEX ------------------------------- */
/* -------------------------------- AUTOINDEX ------------------------------- */
/* -------------------------------- AUTOINDEX ------------------------------- */
/* -------------------------------- AUTOINDEX ------------------------------- */
void Web::__autoindex(Server &__server, Location &__location)
{
	static	int					__l;
	static	int					__s;
	static	int					__lsize;
	__m_iterator				autoindex;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();
	
	if(__values[0] != "on" && __values[0] != "off")
	{
		std::cerr << "\033[1;31m" << "error: wrong autoindex attribute" << "\033[0m" << std::endl;
		exit(1);
	}
	/*CHECK LOCATION*/
	if (__server.__curly_location)
	{
		if (__lsize < __server.__l)
			__l = 0;
		__lsize = __server.__l;
		if(__values.size() > 1 || __l)
		{
			std::cerr << "\033[1;31m" << "error: autoindex attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__l = 1;
		__location.__attributes["autoindex"] = __values;
	}
	else
	{
		if(__values.size() > 1 || __s)
		{
			std::cerr << "\033[1;31m" << "error: autoindex attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__s = 1;

		/*add valuses*/
		__server.__attributes["autoindex"] = __values;
	}
}

/* ------------------------------- ERROR PAGES ------------------------------ */
/* ------------------------------- ERROR PAGES ------------------------------ */
/* ------------------------------- ERROR PAGES ------------------------------ */
/* ------------------------------- ERROR PAGES ------------------------------ */
void Web::__error_page(Server &__server, Location &__location)
{
	(void)__location;
	__m_iterator				error_page;
	std::vector<std::string>	__values;
	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();
	if (__values.size() != 2)
	{
		std::cerr << "\033[1;31m" << "Error : error page attribute" << "\033[0m" << std::endl;
		exit(1);	
	}
	if (!file_exists(__values[1]))
	{
		std::cerr << "\033[1;31m" << "\" " << __values[1] << " \" : file doesn't exist" << "\033[0m" << std::endl;
		exit(1);
	}
	
	/*CHECK LOCATION*/
	if (__server.__curly_location)
	{
		std::cerr << "\033[1;31m" << "error: error pages not allowed in location" << "\033[0m" << std::endl;
		exit(1);
	}
	else
	{
		/*add valuses*/
		if (__server.__attributes.find("error_page") == __server.__attributes.end())
			__server.__attributes["error_page"];
		error_page = __server.__attributes.find("error_page");
		error_page->second.insert(error_page->second.end(), __values.begin(), __values.end());
	}
}

/* ------------------------------- UPLOAD DIR ------------------------------- */
/* ------------------------------- UPLOAD DIR ------------------------------- */
/* ------------------------------- UPLOAD DIR ------------------------------- */
/* ------------------------------- UPLOAD DIR ------------------------------- */
void Web::__upload_dir(Server &__server, Location &__location)
{
	static	int					__l;
	static	int					__s;
	static	int					__lsize;
	__m_iterator				upload_dir;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
	{
		if (__lsize < __server.__l)
			__l = 0;
		__lsize = __server.__l;
		if(__values.size() > 1 || __l)
		{
			std::cerr << "\033[1;31m" << "error: upload_dir attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__l = 1;
		__location.__attributes["upload_dir"] = __values;
	}
	else
	{
		if(__values.size() > 1 || __s)
		{
			std::cerr << "\033[1;31m" << "error: upload_dir attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__s = 1;
		__server.__attributes["upload_dir"] = __values;
	}
}


/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
void Web::__client_body_max_size(Server &__server, Location &__location)
{
	static	int					__s;
	static	int					__l;
	static	int					__lsize;
	__m_iterator				client_body_max_size;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();
	if(__values[0].back() != 'G' && __values[0].back() != 'M' && __values[0].back() != 'B')
	{
		std::cerr << "\033[1;31m" << "error: missing client_body_max_size unit" << "\033[0m" << std::endl;
		exit(1);
	}

	/*CHECK LOCATION*/
	if (__server.__curly_location)
	{
		if (__lsize < __server.__l)
			__l = 0;
		__lsize = __server.__l;
		if(__values.size() > 1 || __l)
		{
			std::cerr << "\033[1;31m" << "error: client_body_max_size attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__l = 1;
		__location.__attributes["client_body_max_size"] = __values;
	}
	else
	{
		if(__values.size() > 1 || __s)
		{
			std::cerr << "\033[1;31m" << "error: client_body_max_size attributes" << "\033[0m" << std::endl;
			exit(1);
		}
		__s = 1;
		client_body_max_size = __server.__attributes.find("client_body_max_size");

		/*add valuses*/
		__server.__attributes["client_body_max_size"] = __values;

	}
}