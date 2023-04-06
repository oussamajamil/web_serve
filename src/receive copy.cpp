/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:05:55 by obelkhad          #+#    #+#             */
/*   Updated: 2023/04/05 22:46:22 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/receive.hpp"

Receive::Receive() : 
__request_read_done(false),
__close(true),
__chunks(false),
__head_read_done(false),
__body_read_done(false),
__requet_read_done(false),
__content_length(0),
__length(0)
{
	__head.resize(10);
	__body.resize(0);
}

Receive::~Receive()
{}

void Receive::__init_requst()
{
	__request_read_done = false;
	__close = false;
	__chunks = false;
	__head_read_done = false;
	__body_read_done = false;
	__content_length = 0;
	__length = 0;
	__head.clear();
	__body.clear();
	__head.resize(10);
	__body.resize(0);
}

void Receive::__request_read(int __client, int __data)
{
	if (!__head_read_done)
	{
		__read_head(__client, __data);
	}
	if (__head_read_done && !__body_read_done && !__chunks)
	{
		// std::cout << __head << std::endl;
		__read_body(__client, __data);
	}
	if (__chunks)
	{
		__read_chunkes_body(__client, __data);
	}
}

void Receive::__read_chunkes_body(int __ident, int &__data)
{
	(void)__ident;
	(void)__data;
	// length := 0
	// read chunk-size, chunk-ext (if any), and CRLF
	// while (chunk-size > 0) {
	// 	read chunk-data and CRLF
	// 	append chunk-data to content
	// 	length := length + chunk-size
	// 	read chunk-size, chunk-ext (if any), and CRLF
	// }
	// read trailer field
	// while (trailer field is not empty) {
	// 	if (trailer fields are stored/forwarded separately) {
	// 		append trailer field to existing trailer fields
	// 	}
	// 	else if (trailer field is understood and defined as mergeable) {
	// 		merge trailer field with existing header fields
	// 	}
	// 	else {
	// 		discard trailer field
	// 	}
	// 	read trailer field
	// }
	// Content-Length := length
	// Remove "chunked" from Transfer-Encoding
}

void Receive::__read_head(int __client, int &__data)
{
	int				__r;
	size_t  		crlf;
	size_t  		__old_pos = __length;
	
	// // std::cout << "-S " << __head.size() << std::endl;
	// // std::cout << "-D " << __data << std::endl;
    // __head.resize(__head.size() + __data);
	// // do
    // // {
	// __r = recv(__client, (void *)(__head.data() + __length), __data, 0);
	// // std::cout << "-R " << __r << std::endl<< std::endl;
	// __length += __r;
	// // std::cout << "-L " << __length << std::endl;
		
	// // std::cout << "-S " << __head.size() << std::endl;

	// if (__r == -1)
	// {
	// 	std::cout << "error: recv(head) \n";
	// 	exit(1);
	// }
	// if (__r == 0)
	// {
	// 	std::cout << "Connection closed by client.d \n";
	// }
    //     // __data -= __r;
    // // } while (__data > 0);
	// // std::cout << "--------------------------------< \n";
    // crlf = __crlf(__head, __old_pos);
	// if (crlf != std::string::npos)
	// {
	// 	__parse_info();
	// 	__head_read_done = true;
	// 	__body = __head.substr(crlf + 4);
	// 	// __body = __head.substr(crlf + 4, crlf + 4 + __content_length);
	// 	__head = __head.substr(0, crlf + 4);
	// 	__length -= crlf + 4;
	// }
	// // std::cout << "- " << __body.size() << std::endl;
	// // // std::cout << "- " << __length << std::endl;
	// // // std::cout << "- " << crlf << std::endl;
	// // std::cout << __head;
	// // std::cout << "--------------------------------< \n";
	// // std::cout << __body;

	// // exit(1);
	//******************************************//


	// std::cout << "__length" << __length << std::endl;
	// std::cout << "__head.size()" << __head.size() << std::endl;exit(1);

	do
    {
		if (__length == __head.length())
			__head.resize(__length * 2);
		
		__r = recv(__client, (void *)(__head.data() + __length), __head.size() - __length, 0);
		__length += __r;
		std::cout << "R: " << __r << std::endl;
		std::cout << "L: " << __length << std::endl;
		if (__r == -1)
		{
			std::cout << "error: recv(head) \n";
			exit(1);
		}
		if (__r == 0)
		{
			std::cout << "Connection closed by client.d \n";
		}
        __data -= __r;
    } while (__data > 0);

    crlf = __crlf(__head, __old_pos);
	if (crlf != std::string::npos)
	{
		__parse_info();
		__head_read_done = true;
		__body = __head.substr(crlf + 4);
		__head = __head.substr(0, crlf + 4);
		__length -= crlf + 4;
	}
	
	std::cout << "<--------------------------------> \n";
	std::cout << "> " << __length << std::endl;
	std::cout << "> " << crlf << std::endl;
	std::cout << "> " << __body.size() << std::endl;
	std::cout << __head;
	std::cout << "<--------------------------------> \n";
	std::cout << __body;

	exit(1);
}

void Receive::__read_body(int __client, int &__data)
{
	int     __r = 0;

	// std::cout << "data  : " << __data << std::endl;
	// std::cout << "size(): " << __body.size() << std::endl;
	// std::cout << "len: " << __length << std::endl;

	// __body.resize(__body.size() + __data);
    // do
	// {
	// 	if (__data > 0)
	// 	{
	// 		if (__data > BUFFER)
	// 			__r = recv(__client, (void *)(__body.data() + __length), BUFFER, 0);
	// 		else
	// 			__r = recv(__client, (void *)(__body.data() + __length), __data, 0);
	// 		std::cout << "R: " << __r << std::endl<< std::endl;
	// 		__length +=  __r;
	// 		if (__r == -1)
	// 		{
	// 			std::cout << "error: recv(body)" << std::endl;
	// 			exit(1);
	// 		}
	// 		if (__r == 0)
	// 		{
	// 			std::cout << "Connection closed by client." << std::endl; 
	// 		}
	// 		__data -= __r;
	// 	}
	// 	if (__content_length == __body.size())
	// 	{
	// 		std::cout << "DONE" << std::endl; 
	// 		__body_read_done = true;
	// 		return;
	// 	}
    // } while (__data > 0);
    do
	{
		if (__data > 0)
		{
			__r = recv(__client, (void *)(__body.data() + __length), __body.size() - __length, 0);
			__length +=  __r;
			if (__r == -1)
			{
				std::cout << "error: recv(body)" << std::endl;
				exit(1);
			}
			if (__r == 0)
			{
				std::cout << "Connection closed by client." << std::endl; 
			}
			__data -= __r;
		}
		if (__length == __body.length())
		{
			__body_read_done = true;
			return;
		}
    } while (__data > 0);
}

std::string Receive::__search_str(std::string __str)
{
	size_t		pos;

	pos = __head.find(__str);
	if (pos != std::string::npos)
	{
		pos += __str.length();
		return (__head.substr(pos, __head.find("\r\n", pos) - pos));
	}
	return (NPOS);
}

void Receive::__parse_info()
{
	std::string	__holder;
	/* ---------------------------- Transfer Encoding --------------------------- */
	__holder = __search_str("Transfer-Encoding: ");
	if (__holder != NPOS && __holder == "chunked")
	{
		__chunks = true;
	}
	else
	{	
		/* ----------------------------- Content-Length ----------------------------- */
		__holder = __search_str("Content-Length: ");
		if (__holder != NPOS)
			__content_length = stoi(__holder);
		else
			__content_length = 0;	
	}
	
	/* ------------------------------- keep-alive ------------------------------- */
	__holder = __search_str("Connection: ");
	if (__holder != NPOS && __holder == "keep-alive")
		__close = false;

	/* --------------------------------- Server --------------------------------- */
	__holder = __search_str("Host: ");
	
	if (__holder == NPOS)
	{
		std::cout << "error: bad requset HOST missing!" << std::endl;
		exit(1);
	}
	else
		__host = __holder;
}