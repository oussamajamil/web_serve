/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utl_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 16:12:30 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/04 16:26:04 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/server.hpp"
#include "../include/utils.hpp"

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
	__m_iterator				methods;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);
	
	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
	{		
		__location.__attributes["methods"].insert(__location.__attributes["methods"].end(), __values.begin(), __values.end());
	}
	else
	{
		methods = __server.__attributes.find("methods");

		/*remove default configuration*/
		if (__server.__methods_default)
		{
			methods->second.clear();
			__server.__methods_default = false;
		}

		/*add valuses*/
		methods->second.insert(methods->second.end(), __values.begin(), __values.end());
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

	/*CHECK LOCATION*/
	if (__server.__curly_location)
		__location.__attributes["cgi"].insert(__location.__attributes["cgi"].end(), __values.begin(), __values.end());
	else
	{
		cgi = __server.__attributes.find("cgi");


		/*add valuses*/
		cgi->second.insert(cgi->second.end(), __values.begin(), __values.end());
	}
}

/* ---------------------------------- ROOT ---------------------------------- */
/* ---------------------------------- ROOT ---------------------------------- */
/* ---------------------------------- ROOT ---------------------------------- */
/* ---------------------------------- ROOT ---------------------------------- */
void Web::__root(Server &__server, Location &__location)
{
	__m_iterator				root;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
		__location.__attributes["root"].insert(__location.__attributes["root"].end(), __values.begin(), __values.end());
	else
	{
		root = __server.__attributes.find("root");

		/*remove default configuration*/
		if (__server.__root_default)
		{
			root->second.clear();
			__server.__root_default = false;
		}

		/*add valuses*/
		root->second.insert(root->second.end(), __values.begin(), __values.end());
	}
}

/* ---------------------------------- INDEX --------------------------------- */
/* ---------------------------------- INDEX --------------------------------- */
/* ---------------------------------- INDEX --------------------------------- */
/* ---------------------------------- INDEX --------------------------------- */
void Web::__index(Server &__server, Location &__location)
{
	__m_iterator				index;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
		__location.__attributes["index"].insert(__location.__attributes["index"].end(), __values.begin(), __values.end());
	else
	{
		index = __server.__attributes.find("index");

		/*remove default configuration*/
		if (__server.__index_default)
		{
			index->second.clear();
			__server.__index_default = false;
		}

		/*add valuses*/
		index->second.insert(index->second.end(), __values.begin(), __values.end());
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
	__m_iterator				autoindex;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
		__location.__attributes["autoindex"].insert(__location.__attributes["autoindex"].end(), __values.begin(), __values.end());
	else
	{
		autoindex = __server.__attributes.find("autoindex");

		/*remove default configuration*/
		if (__server.__autoindex_default)
		{
			autoindex->second.clear();
			__server.__autoindex_default = false;
		}

		/*add valuses*/
		autoindex->second.insert(autoindex->second.end(), __values.begin(), __values.end());
	}
}

/* ------------------------------- ERROR PAGES ------------------------------ */
/* ------------------------------- ERROR PAGES ------------------------------ */
/* ------------------------------- ERROR PAGES ------------------------------ */
/* ------------------------------- ERROR PAGES ------------------------------ */
void Web::__error_page(Server &__server, Location &__location)
{
	__m_iterator				error_page;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
		__location.__attributes["error_page"].insert(__location.__attributes["error_page"].end(), __values.begin(), __values.end());
	else
	{
		error_page = __server.__attributes.find("error_page");

		/*remove default configuration*/
		if (__server.__error_page_default)
		{
			error_page->second.clear();
			__server.__error_page_default = false;
		}

		/*add valuses*/
		error_page->second.insert(error_page->second.end(), __values.begin(), __values.end());
	}
}

/* ------------------------------- UPLOAD DIR ------------------------------- */
/* ------------------------------- UPLOAD DIR ------------------------------- */
/* ------------------------------- UPLOAD DIR ------------------------------- */
/* ------------------------------- UPLOAD DIR ------------------------------- */
void Web::__upload_dir(Server &__server, Location &__location)
{
	__m_iterator				upload_dir;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
		__location.__attributes["upload_dir"].insert(__location.__attributes["upload_dir"].end(), __values.begin(), __values.end());
	else
	{
		upload_dir = __server.__attributes.find("upload_dir");

		/*remove default configuration*/
		if (__server.__upload_dir_default)
		{
			upload_dir->second.clear();
			__server.__upload_dir_default = false;
		}

		/*add valuses*/
		upload_dir->second.insert(upload_dir->second.end(), __values.begin(), __values.end());
	}
}


/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
/* ----------------------------- CLIENT MAX SIZE ---------------------------- */
void Web::__client_body_max_size(Server &__server, Location &__location)
{
	__m_iterator				client_body_max_size;
	std::vector<std::string>	__values;

	/*initial action*/
	__initial_action(__server);

	/*extract values*/
	__values = __parse_args();

	/*CHECK LOCATION*/
	if (__server.__curly_location)
		__location.__attributes["client_body_max_size"].insert(__location.__attributes["client_body_max_size"].end(), __values.begin(), __values.end());
	else
	{
		client_body_max_size = __server.__attributes.find("client_body_max_size");

		/*remove default configuration*/
		if (__server.__client_body_max_size_default)
		{
			client_body_max_size->second.clear();
			__server.__client_body_max_size_default = false;
		}

		/*add valuses*/
		client_body_max_size->second.insert(client_body_max_size->second.end(), __values.begin(), __values.end());
	}
}