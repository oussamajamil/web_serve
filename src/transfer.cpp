/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:05:55 by obelkhad          #+#    #+#             */
/*   Updated: 2023/04/27 10:38:27 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/transfer.hpp"


Transfer::Transfer() : 
__request_read_done(false),
__close(true),
__chunks(false),
__read_done(false),
__head_read_done(false),
__requet_read_done(false),
__response_send_done(false),
__keep(false),
__content_length(0),
__length(0),
__length_s_(0),
__rest(0),
__sub_chunck(0),
__res_buff_len(0)
{
	__request.clear();
	__request.resize(BUFFER);
	__head.clear();
	__body.clear();
}

Transfer::~Transfer()
{}

void Transfer::__init_requst()
{
	__request_read_done = false;
	__close = false;
	__chunks = false;
	__read_done = false;
	__head_read_done = false;
	__response_send_done = false;
	__keep = false;
	__content_length = 0;
	__res_buff_len = 0;
	__length = 0;
	__length_s_ = 0;
	__rest = 0;
	__sub_chunck = 0;
	__res_buff.clear();
	__request.clear();
	__request.resize(BUFFER);
	__head.clear();
	__body.clear();
}

void Transfer::__request_read(int __client, int __data)
{
	if (!__read_done)
	{
		__read_(__client, __data);
	}
}

int Transfer::__response_send(int __client)
{
	int __s = 0;

	__s = send(__client, (void *)(__res_buff.data() + __length_s_), __res_buff_len - __length_s_, 0);
	if (__s == -1)
		return -1;
	__length_s_ += __s;
	if (__length_s_ == __res_buff_len)
	{
		__response_send_done = true;
		return 1;
	}
	return 1;
}

void Transfer::__read_(int __client, int &__data)
{
	int				__r;
	size_t			__p;
	size_t			__l = 0;
	std::string		__num;
	size_t  		__old_pos = __length;
	std::string		__hold;

	if (__length == __request.length())
		__request.resize(__length * 2);
	__r = recv(__client, (void *)(__request.data()), __request.length() - __length, 0);


	if (__r == -1)
		return;

	__length += __r;
	__request.resize(__r);
	size_t __s = __length;
	if (__head_read_done == false)
	{
		size_t crlf;

		__head.append(__request.c_str(), __r);
		crlf = __crlf(__request, __old_pos);
		if (crlf != std::string::npos)
		{
			__head_read_done = true;
			__head = __request.substr(0, crlf + 4);
			__parse_info();
			if (!__chunks)
				__body = __request.substr(crlf + 4);
			__request.erase(0, crlf + 4);
			__length -= crlf + 4;
			if (__chunks)
			{
				__read_chunkes(__p, __l, __num, __hold);
			}
		}
	}
	else if (__chunks)
	{
		if (__sub_chunck > 0)
		{
			__hold = __request.substr(0, __sub_chunck);
			__body.append(__request.substr(0, __sub_chunck));
			if(__sub_chunck > __hold.length())
			{
				__request.erase(0, __sub_chunck);
				__sub_chunck -= __hold.length();
			}
			else
			{
				__request.erase(0, __sub_chunck + 2);
				__sub_chunck = 0;
			}
		}
		__read_chunkes(__p, __l, __num, __hold);
	}
	else
	{
		__body.append(__request.c_str(), __r);
	}
	if (!__chunks && __content_length == __length)
	{
		__read_done = true;
		return;
	}
	__request.clear();
	__request.resize(__s);
	__data -= __r;
}

void Transfer::__read_chunkes(size_t &__p, size_t &__l, std::string &__num, std::string &__hold)
{
	while (__request.length() > 0)
	{
		__p = __request.find("\r\n");
		__num = __request.substr(0, __p);
		__l = stoi(__num, 0, 16);
		if (__l == 0)
		{
			__read_done = true;
			return;
		}
		__request.erase(0, __p + 2);
		__hold = __request.substr(0, __l);
		__body.append(__request.substr(0, __l));
		__request.erase(0, __l + 2);
	}
	if(__l > __hold.length())
		__sub_chunck = __l - __hold.length();
}
std::string Transfer::__search_str(std::string __str)
{
	size_t pos;

	pos = __head.find(__str);
	if (pos != std::string::npos)
	{
		pos += __str.length();
		return (__head.substr(pos, __head.find("\r\n", pos) - pos));
	}
	return (NPOS);
}

void Transfer::__parse_info()
{
	std::string __holder;

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
		{
			__read_done = true;
			__content_length = 0;
		}
	}

	/* ------------------------------- keep-alive ------------------------------- */
	__holder = __search_str("Connection: ");
	if (__holder == NPOS)
		__keep = true;
	else if (__holder == "keep-alive")
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