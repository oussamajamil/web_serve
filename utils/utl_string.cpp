/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utl_string.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:49:57 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/27 15:48:55 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.hpp"
#include "../include/server.hpp"

std::string __config_file(int ac, char **av)
{
	if (ac == 1)
		return ("config/default.conf");
	else if (ac > 2)
		__bad_number_arguments();
	else
		__extention_is_good(av[1]);
	return (av[1]);
}

std::vector<std::string> __split_attrubites(std::string str, const std::string delims)
{
    std::vector<std::string> tokens;
    size_t pos = 0;

	while ((pos = str.find_first_of(delims)) != std::string::npos)
	{
		
		if (!str.substr(0, pos).empty())
			tokens.push_back(str.substr(0, pos));
		if (str[pos] == '{')
        	tokens.push_back("{");
		else if (str[pos] == '}')
        	tokens.push_back("}");
		if (str[pos] == ';')
        	tokens.push_back(";");
        str.erase(0, pos + 1);
    }
	if (!str.empty())
    	tokens.push_back(str);
    return tokens;
}

std::string __trim(std::string &expretion)
{
	expretion.erase(0, expretion.find_first_not_of(" \t\r\v\f"));
	expretion.erase(expretion.find_last_not_of(" \t\r\v\f") + 1);
	return expretion;
}

std::string __extract_parameters(std::string &line)
{
	// Ignore comments and empty lines
	if (line.empty() || line[0] == '#')
		return "";

	// Remove trailing comments
	size_t comment_pos = 0;
	if ((comment_pos = line.find('#')) != std::string::npos)
		line.erase(comment_pos);

	// Trim leading and trailing spaces
	line = __trim(line);

	return line;
}

void __vector_display(std::vector<std::string> &vec)
{
	std::vector<std::string>::iterator it = vec.begin();
	for (; it != vec.end(); ++it)
		std::cout << "[ " << *it << " ], ";
	std::cout << std::endl;
	// std::cout << std::endl;
}

void __map_display(std::map<std::string, std::vector<std::string> > &mp)
{
	int __x = 0;
	std::map<std::string, std::vector<std::string> >::iterator it = mp.begin();
	for (; it != mp.end(); ++it)
	{	
		__x = it->first.length();
		std::cout << "	-> " << it->first;
		for(int i = 0; i < 30 - __x;i++)
			std::cout << ".";
		__vector_display(it->second);
	}
}

void __server_display(Server server)
{
	int __x = 0;
	std::cout << std::endl << "-------------------------------------  SERVER START  --------------------------------------------" << std::endl;
	std::cout << "ATTRIBUTES : " << server.__attributes.size() << std::endl;
	std::cout << "LOCATIONS : " << server.__locations.size() << std::endl << std::endl;

	std::cout << "ATTRIBUTES : " << std::endl;
	
	std::map<std::string, std::vector<std::string> >::iterator it = server.__attributes.begin();
	for (; it != server.__attributes.end(); ++it)
	{	
		__x = it->first.length();
		std::cout << "-> " << it->first;
		for(int i = 0; i < 30 - __x;i++)
			std::cout << ".";
		__vector_display(it->second);
	}
	std::cout << std::endl;

	std::cout << "LOCATIONS : " << std::endl;

	std::vector<Location>::iterator					__vec_loc = server.__locations.begin();

	for (; __vec_loc != server.__locations.end(); ++__vec_loc)
	{	
		std::cout << ">>> LOCATION : " << __vec_loc->__path << std::endl;
		__map_display(__vec_loc->__attributes);
	}
	std::cout << "-------------------------------------------------  SERVER END  ----------------------------------------------------" << std::endl;
}

void __servers_display(Web &web)
{

	std::vector<Server>::iterator it = web.__servers.begin();
	for (; it != web.__servers.end(); ++it)
	{
		__server_display(*it);
	}
}