/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:50:19 by obelkhad          #+#    #+#             */
/*   Updated: 2023/04/27 10:41:06 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TRANSFER_HPP
#define TRANSFER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <unistd.h>
#include "../include/server.hpp"
#include "../include/utils.hpp"

class Transfer
{
public:
	bool							__request_read_done;
	bool							__close;
	bool							__chunks;
	bool							__read_done;
	bool							__head_read_done;
	bool							__requet_read_done;
	bool							__response_send_done;
	bool							__keep;

	int								__ident;
	int								__scoket;

	size_t							__content_length;
	size_t							__length;
	size_t							__length_s_;
	size_t							__rest;
	size_t							__sub_chunck;
	size_t							__res_buff_len;

	std::string						__res_buff;
	std::string						__request;
	std::string						__head;
	std::string						__body;
	std::string						__host;

	Server							*__server;

	struct sockaddr_in				__addr;

	Transfer();
	~Transfer();

	void							__init_requst();
	void							__request_read(int, int);
	int								__response_send(int);
	void							__read_(int, int &);
	void							__read_body(int, int &);
	void							__read_chunkes(size_t &, size_t &, std::string &, std::string &);
	void							__parse_info();
	std::string						__search_str(std::string);
};






#endif