/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utl_string.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:49:57 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/01 11:19:15 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.hpp"
#include "../include/errors.hpp"

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
	std::cout << "vec >> ";
	for (; it != vec.end(); ++it)
		std::cout << "[ " << *it << " ], ";
	std::cout << '\n';
	std::cout << "-------------------" << std::endl;
}


// void __open_curly_bracket( bool __server, bool __loc_zone, bool __location)
// {
// 	// if (__server && __loc_zone && __location\
// 	// 	|| __server && !__loc_zone )
// 	// {
// 	// 	std::cerr << "error: Curly Bracket not closed!." << std::endl;
// 	// 	exit(1);
// 	// }
// 	// if (!__server)
// 	// 	__server = true;
// 	// if (__server && !__loc_zone)
// 	// 	__location = true;
// }

// void __close_curly_bracket( bool __server, bool __loc_zone, bool __location)
// {
// }