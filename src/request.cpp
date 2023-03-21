/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:05:55 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/21 18:17:54 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/request.hpp"

Request::Request() : 
__request_read_done(false),
__close(true),
__chunks(false),
__head_read_done(false),
__body_read_done(false),
__requet_read_done(false),
__content_length(0),
__length(0)
{
	__head.resize(BUFFER);
	__body.resize(BUFFER);
}

Request::~Request()
{}

void Request::__init_requst()
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
	__head.resize(BUFFER);
	__body.resize(BUFFER);
}

void Request::__request_read(int __ident, int __data)
{
	if (!__head_read_done)
		__read_head(__ident, __data);
	if (!__body_read_done)
		__read_body(__ident, __data);
}

void Request::__read_head(int __ident, int &__data)
{
	int				__r = 0;
	size_t  		crlf;
	size_t  		__old_pos = __length;

	do
    {
        if (__length == __head.length())
            __head.resize(__length * 2);
        __r = recv(__ident, (void *)(__head.data() + __length), __head.size() - __length, 0);
		__length += __r;
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
		__head_read_done = true;
		__body = &__head[crlf + 4];
		__head = __head.substr(0, crlf + 4);
		__parse_info();
		__length -= crlf + 4;
	}
}

void Request::__read_body(int __ident, int &__data)
{
	int     __r = 0;
    
    do
    {
        if (__data > 0 && __content_length)
        {
			if (__length == __body.length())
        		__body.resize(__length * 2);
            __r = recv(__ident, (void *)(__body.data() + __length), __body.size() - __length, 0);
			__length +=  __r;
			if (__r == -1)
			{
				std::cout << "error: recv()" << std::endl;
				exit(1);
			}
			if (__r == 0)
			{
				std::cout << "Connection closed by client." << std::endl;
			}
			__data -= __r;
			if (__content_length == __length)
			{
				__body_read_done = true;
				return;
			}
        }
    } while (__data > 0);
}

std::string Request::__search_str(std::string __str)
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

void Request::__parse_info()
{
	std::string	__holder;
	
	/* ----------------------------- Content-Length ----------------------------- */
	__holder = __search_str("Content-Length: ");
	if (__holder != NPOS)
		__content_length = stoi(__holder);
	else
	{
		__content_length = 0;
		// __requet_read_done = true;
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
	{
		__host = __holder;
	}
}