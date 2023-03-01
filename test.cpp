/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:47:12 by obelkhad          #+#    #+#             */
/*   Updated: 2023/02/28 20:40:40 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>

#include <sstream>
void __parse_adress(std::string input)
{
    // std::string input = "192.168.1.1:8080";
    // std::regex regex("^([0-9]{1,3}\\.){3}[0-9]{1,3}:[0-9]{1,5}$");

    // if (std::regex_match(input, regex)) {
    //     size_t pos = input.find(":");
    //     std::string ip = input.substr(0, pos);
    //     std::string port = input.substr(pos + 1);

    //     std::cout << "IP address: " << ip << std::endl;
    //     std::cout << "Port number: " << port << std::endl;
    // } else {
    //     std::cerr << "Invalid input: " << input << std::endl;
    // }

	// std::string			__holder_str = str;
	// std::string			__holder;
    // size_t 				__pos;
    // size_t 				__p;
	// int 				__num;

	// int 				__i = 0;
	// size_t 				__range;


	// while (__holder_str[__i])
	// {
	// 	std::cout << "[ H ]  :" << __holder_str << std::endl;
	// 	__pos = 0;
	// 	if ((__p = __holder_str.find('.')) == std::string::npos)
	// 		__holder = __holder_str;
	// 	else
	// 		__holder = __holder_str.substr(0, __p);
	// 	__num = std::stoi(__holder, &__pos);
	// 	std::cout << "[ N ]  :" << __num << std::endl;
		
	// 	if (__pos != __holder.length())
	// 	{
	// 		std::cerr << "Invalid input: " << __holder << std::endl;
	// 	}
	// 	else
	// 	{
	// 		std::cout << "The integer value is: " << __num << std::endl;
	// 	}
	// 	__holder_str = __holder_str.substr(__holder_str.find('.') + 1);
	// }

	// std::cout << "h : " << __holder << '\n';
	// __range = stoi(__holder);
	// std::cout << "r : " << __range << '\n';
	// if (__range < 0 || __range > 255)
	// 	std::cout << "error " << '\n';

	
}



int main(int ac, char **av)
{
	// parse config file;
	__parse_adress(av[1]);
	
    return 0;
}



// 
// #include <arpa/inet.h>
//
	// struct in_addr 		__addr;	
    // int 				__res;

	// __res = inet_pton(AF_INET, ip_str.c_str(), &__addr);
    // if (__res == 1)
	// {
    //     std::cout << "Valid IP address\n";
    // } else if (__res == 0) {
    //     std::cout << "Invalid IP address (not in presentation format)\n";
    // } else {
    //     std::cout << "Invalid IP address\n";
    // }