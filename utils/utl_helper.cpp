/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utl_helper.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 17:18:53 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/20 11:00:50 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <arpa/inet.h>
#include "../include/define.hpp"


void __address_prot_form(std::string &address)
{
	std::string 	__ip;
	struct in_addr	__addr;
	size_t			__pos;
	int 			__res;
	
	__pos = address.find(":");
	if (__pos != std::string::npos)
		__ip = address.substr(0, __pos);
	else
	{
		__ip = address;
		address = address + ":" + PORT;
	}
	//TODO: change inet_pton to getaddrinfo()

	__res = inet_pton(AF_INET, __ip.c_str(), &__addr);
	if (__res != 1)
	{
		std::cerr << "\033[1;31m" << "error : Invalid IP address" << "\033[0m" << std::endl;
		exit (1);
	}
}

size_t __crlf(std::string &str, size_t pos)
{
    size_t _ret;

    _ret = str.find("\r\n\r\n", pos);
    if (_ret != std::string::npos)
        return (_ret);
    return (std::string::npos);
}