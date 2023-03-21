/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:50:19 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/21 18:40:11 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REVEIVE_HPP
#define REVEIVE_HPP

#include <sys/socket.h>
#include "../include/server.hpp"
#include "../include/utils.hpp"

class Request
{
public:
	bool							__request_read_done;
	bool							__close;
	bool							__chunks;
	bool							__head_read_done;
	bool							__body_read_done;
	bool							__requet_read_done;

	int								__ident;
	int								__scoket;

	size_t							__content_length;
	size_t							__length;

	std::string						__head;
	std::string						__body;
	std::string						__host;

	Server							*__server;

	Request();
	~Request();

	void							__init_requst();
	void							__request_read(int __ident, int __data);
	void							__read_head(int __iden, int &__data);
	void							__read_body(int __ident, int &__data);
	void							__parse_info();
	
	std::string						__search_str(std::string);

	/* --------------------------------- GETERS --------------------------------- */
};






#endif