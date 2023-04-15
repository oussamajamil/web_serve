/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:05:55 by obelkhad          #+#    #+#             */
/*   Updated: 2023/04/12 23:46:15 by obelkhad         ###   ########.fr       */
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
	__content_length = 0;
	__res_buff_len = 0;
	__length = 0;
	__length_s_ = 0;
	__rest = 0;
	__sub_chunck = 0;
	// __res_buff = NULL;
	__res_buff.clear();
	__request.clear();
	__request.resize(BUFFER);
	__head.clear();
	__body.clear();
}

void Transfer::__request_read(int __client, int __data)
{
// 	if (__chunks)
// 	{
// 		__read_chunkes(__client, __data);
// 	}
// 	else if (!__read_done)
// 	{
// 		__read_(__client, __data);
	if (!__read_done)
	{
		__read_(__client, __data);
	}
}

void Transfer::__response_send(int __client, int __data)
{
	int __s = 0;
	// int	__buf = BUFFER;
    // do
    // {
	// std::cout << "s__client :: " << __client << std::endl;
	// // std::cout << "s__data :: " << __data << std::endl;
	// std::cout << "s__length_s_ :: " << __length_s_ << std::endl;
	// std::cout << "s__res_buff_len :: " << __res_buff_len << std::endl;
	// std::cout << "s__res_buff :: " << __res_buff << std::endl;
	// std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ " << std::endl;
	// std::cout << "s__res_buff :: " << __res_buff[__length_s_] << std::endl;
	if ( __data > 0)
	{
		
		__s = send(__client, (void *)(&__res_buff[__length_s_]), __res_buff_len - __length_s_, 0);
		// __s = send(__client, (void *)(&__res_buff[__length_s_]), BUFFER * 2, 0);
		// std::cout << "s__s :: " << __s << std::endl;
		if (__s == -1)
		{
			std::cout << "Error: send() \n";
			return ;
		}
		// if (__s == 0)
		// {
		// 	std::cout << "Connection closed by client send ()\n";
		// }
		// if (__s > 0)
		{
			__length_s_ += __s;
			__data -= __s;
		}
		// std::cout << "s__data :: " << __data << std::endl<< std::endl;
		// std::cout << "s__length_s_ :: " << __length_s_ << std::endl << std::endl;
		if (__length_s_ == __res_buff_len)
		{
			__response_send_done = true;
			return ;
		}
	}
    // } while (__data > 0);
}

void Transfer::__read_(int __client, int &__data)
{
	int				__r;
	size_t  		crlf;
	size_t  		__old_pos = __length;

	// std::cout << "r__client :: " << __client << std::endl;
	// std::cout << "r__data :: " << __data << std::endl;
	// std::cout << "r__request.length() :: " << __request.length() << std::endl;
	// std::cout << "r__length :: " << __length << std::endl;

	// do
	// {
		if (__length == __request.length())
            __request.resize(__length * 2);
		if (__data > 0)
		{	
			// if ((int)(__request.size() - __length) <= __data)
			__r = recv(__client, (void *)(__request.data()), __request.length() - __length, 0);
			// else if (__data > 0)
			// 	__r = recv(__client, (void *)(__request.data()), __data, 0);
			// std::cout << "r__r :: " << __r << std::endl;
			if (__r == -1)
			{
				std::cout << "Error: recv() \n";
				return;
			}
			if (__r == 0)
			{
				std::cout << "Connection closed by client\n";
			}
			__length += __r;
			__request.resize(__length);
			size_t	__s = __request.size();
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
					if (__chunks)
					{
						int	__l = stoi(__body.substr(0, __body.find("\r\n")));
						__body.erase(0, __body.find("\r\n") + 2);
						__body.erase(0, 2);
						__sub_chunck = __l - __body.length();
						if (__sub_chunck < 0)
							__rest = __l;
						// __length_chunck += __sub_chunck;
					}
				}	
			}
			else if (__chunks)
			{
				if (__rest)
				{
					__request.insert(0, __body.substr(__rest + 2));
					__body.erase(__rest, __body.length() - __rest);
				}
				__read_chunkes();
			}
			else
			{	
				__body.append(__request.c_str(), __r);
			}
			// std::cout << "__length :: " << __length << std::endl<< std::endl;
			if (__content_length == __length)
			{
				__read_done = true;
				return;
			}
			__request.clear();
			__request.resize(__s);
			__data -= __r;
		}
		std::cout << "head :: " << __head << std::endl<< std::endl;
	// }while(__data > 0);
}

void Transfer::__read_chunkes()
{
	int	__l;

	while (__request.length() > 0)
	{
		if (__sub_chunck > 0)
		{
			__body.append(__request.c_str(), __sub_chunck);
			__request.erase(0, __sub_chunck + 2);
		}
		if (__request.length() > 0)
		{
			__l = stoi(__request.substr(0, __request.find("\r\n")));
			__request.erase(0, __request.find("\r\n") + 2);;
			__request.erase(0, 2);
			if (__request.length() > (size_t)(__l + 2))
			{
				__body.append(__request.c_str(), __l);
				__request.erase(0, __l + 2);
			}
			else
			{
				__body.append(__request.c_str(), __request.length());
				__request.erase(0, __request.length());
				__sub_chunck = __l - __request.length();
			}
		}
	}
}

std::string Transfer::__search_str(std::string __str)
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

void Transfer::__parse_info()
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
		{
			__read_done = true;
			__content_length = 0;	
		}
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