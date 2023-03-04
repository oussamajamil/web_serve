/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utl_helper.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 17:18:53 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/02 18:33:15 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <arpa/inet.h>
#include "../include/default.hpp"


void __address_prot_form(std::string &address)
{
	std::string 	__ip;
	struct in_addr	__addr;
	size_t			__pos;
	int 			__res;
	static int		__port_inc = std::stoi(PORT);
	
	__pos = address.find(":");
	if (__pos != std::string::npos)
		__ip = address.substr(0, __pos);
	else
	{
		__ip = address;
		address = address + ":" + std::to_string(__port_inc++);
	}
	__res = inet_pton(AF_INET, __ip.c_str(), &__addr);
	if (__res != 1)
	{
		std::cerr << "\033[1;31m" << "error : Invalid IP address" << "\033[0m" << std::endl;
		exit (1);
	}
}
