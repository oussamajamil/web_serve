/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:47:12 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/21 16:19:11 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/server.hpp"
#include "include/utils.hpp"
#include "include/errors.hpp"
#include "include/launch.hpp"


int main(int ac, char **av)
{
	// parse config file;
	Web servers;
	try
	{
		std::string __config = __config_file(ac, av);

		servers.__parse(__config);
		
		Server_launch	__launcher(&servers.__servers);

		__launcher.__launch();
		__launcher.__run();
	}
	
	catch(const std::exception& e)
	{
		std::cerr << "\033[1;31m" << "error: bad synatx" << "\033[0m" << std::endl;
	}

	
	
    return 0;
}
