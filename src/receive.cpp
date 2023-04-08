/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:05:55 by obelkhad          #+#    #+#             */
/*   Updated: 2023/04/08 02:57:09 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/receive.hpp"

Receive::Receive() : 
__request_read_done(false),
__close(true),
__chunks(false),
__read_done(false),
__head_read_done(false),
__requet_read_done(false),
__b(false),
__content_length(0),
__length(0)
{
	__request.clear();
	__request.resize(BUFFER);
	__head.clear();
	__body.clear();
}

Receive::~Receive()
{}

void Receive::__init_requst()
{
	__request_read_done = false;
	__close = false;
	__chunks = false;
	__read_done = false;
	__head_read_done = false;
	__content_length = 0;
	__length = 0;
	__request.clear();
	__head.clear();
	__body.clear();
	// __head.resize(0);
	// __body.resize(0);
}

void Receive::__request_read(int __client, int __data)
{
	if (!__read_done)
	{
		__read_(__client, __data);
	}
	else if (__chunks)
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

void Receive::__read_(int __client, int &__data)
{
	size_t			__r;
	size_t  		crlf;
	size_t  		__old_pos = __length;

	do
	{
		if (__length == __request.length())
            __request.resize(__length * 2);
		if (__request.size() - __length <= (size_t)__data)
			__r = recv(__client, (void *)(__request.data()), __request.length() - __length, 0);
		else
			__r = recv(__client, (void *)(__request.data()), __data, 0);
		if (__r == (size_t)-1)
		{
			std::cout << "Error: recv() \n";
			exit(1);
		}
		if (__r == 0)
		{
			std::cout << "Connection closed by client\n";
		}
		__length += __r;
        __request.resize(__length);
		if (__head_read_done == false)
		{
			__head.append(__request.c_str(), __r);
			crlf = __crlf(__request, __old_pos);
			if (crlf != std::string::npos)
			{
				__head_read_done = true;
				__body = __head.substr(crlf + 4);
				__head = __request.substr(0, crlf + 4);
				__parse_info();
				__length -= crlf + 4;
			}	
		}
		else
		{
			__body.append(__request.c_str(), __r);
		}
		if (__content_length == __length)
		{
			__read_done = true;
			return;
		}
		size_t	__s = __request.size();
		__request.clear();
		__request.resize(__s);
		__data -= __r;
	}while(__data > 0);
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