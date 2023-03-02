/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 12:16:26 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/01 15:37:23 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/errors.hpp"

void __unclosed_curly()
{

	std::cerr << "\033[1;31m" << "error: unclosed curly bracket!." << "\033[0m" << std::endl;
	exit(1);
}

void __curly_right_check(std::string __curly)
{
	if (__curly != "{")
	{
		std::cerr << "\033[1;31m" << "error: opened curly bracket missing!." << "\033[0m" << std::endl;
		exit(1);
	}
}

void __file_fail(std::ifstream	&__file)
{
	if (__file.fail())
	{
		std::cerr << "\033[1;31m" << "error: Fail to opean configuration file!." << "\033[0m" << std::endl;
		exit(1);
	}
}

void __bad_number_arguments()
{
    std::cerr << "\033[1;31m" << "error: bad number of arguments!." << "\033[0m" << std::endl;
    exit(1);
}

void __extention_is_good(std::string str)
{
	if (str.find(".conf") == std::string::npos)
	{
		std::cerr << "\033[1;31m" << "error: bad config file extension!." << "\033[0m" << std::endl;
		exit(1);
	}
}

void __semi_colon_missing()
{
	std::cerr << "\033[1;31m" << "error: semi colon missing!."  << "\033[0m" << std::endl;
	exit(1);
}

void __attributes_missing()
{
	std::cerr << "\033[1;31m" << "error: attributes missing!." << "\033[0m" << std::endl;
	exit(1);
}

void __wrong_arg()
{
	std::cerr << "\033[1;31m" << "error: argument in worng zone!." << "\033[0m" << std::endl;
	exit(1);
}

void __bad_syntax()
{
	std::cerr << "\033[1;31m" << "error: bad syntax!." << "\033[0m" << std::endl;
	exit(1);
}