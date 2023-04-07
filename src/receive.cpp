/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:05:55 by obelkhad          #+#    #+#             */
/*   Updated: 2023/04/07 17:19:54 by obelkhad         ###   ########.fr       */
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
	int				__r;
	size_t  		crlf;
	size_t  		__old_pos = __length;
	char			buff[BUFFER];

	(void)__data;

	// std::cout  << std::endl << std::endl;
	// std::cout << "cl  : " << __content_length << std::endl;
	// std::cout << "data  : " << __data << std::endl;
	// std::cout << "size(): " << __body.size() << std::endl;
	// std::cout << "len: " << __length << std::endl;
	// std::cout << "__head_read_done: " << __head_read_done << std::endl;
	// std::cout << "__client: " << __client << std::endl;
	// std::cout << "BUFFER: " << BUFFER << std::endl;

	bzero(&buff, BUFFER);
	do{
		__r = recv(__client, buff, BUFFER, 0);
		if (__r == -1)
		{
			std::cout << "Error: recv() \n";
			exit(1);
		}
		if (__r == 0)
		{
			std::cout << "Connection closed by client\n";
		}
		__length += __r;
		if (!__head_read_done)
		{
			__head.append(buff, __r);
			// std::cout << __head << std::endl;
			// std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
			crlf = __crlf(__head, __old_pos);
			if (crlf != std::string::npos)
			{
				__parse_info();
				__head_read_done = true;
				__body = __head.substr(crlf + 4);
				__head = __head.substr(0, crlf + 4);
				// std::cout << __head << std::endl;
				// std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@####" << std::endl;
				// std::cout << __body << std::endl;
				__length -= crlf + 4;
			}	
		}
		else
		{
			__body.append(buff, __r);
			// std::cout << "CL : " << __content_length << std::endl
			// << "Length : " << __length << std::endl;
			if (__content_length <= __length)
			{
				__read_done = true;
				return;
			}
		}
		__data -= __r;
	}while(__data > 0);
}

void Receive::__read_body(int __client, int &__data)
{
	int     __r = 0;

	// char *buff = {0};
	//	r = recv(buff);
	//
	// if ("\r\n\r\n" == ila makanch)
	// {
		// 
	// 		head.append(buff, r)
	//		content
	// }
	//	else
	//		body.append(buff, r);
	

	// std::cout << "cl  : " << __content_length << std::endl;
	// std::cout << "data  : " << __data << std::endl;
	// std::cout << "size(): " << __body.size() << std::endl;
	// std::cout << "len: " << __length << std::endl;

	__body.resize(__body.size() + __data);
	// std::cout << "AF size(): " << __body.size() << std::endl;

	// std::cout << "OO: " <<  __body.size() - __length << std::endl;
	__r = recv(__client, (void *)(__body.data() + __length), __data, 0);
	std::cout << "__r (Body): " << __r << std::endl << std::endl;
	
	__length +=  __r;
	// std::cout << "__ll: " << __length << std::endl << std::endl;

	if (__r == -1)
	{
		std::cout << "****** error: recv(--body)" << std::endl;
		exit(1);
	}
	if (__r == 0)
	{
		std::cout << "Connection closed by client." << std::endl; 
	}
	__data -= __r;

	if (__content_length == __length + 1)
	{
		// std::cout << "DONE" << std::endl; 
		__read_done = true;
		return;
	}
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