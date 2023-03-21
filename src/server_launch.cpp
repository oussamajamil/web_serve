/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_launch.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:14:44 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/21 19:50:18 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/launch.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>


/* ------------------------------ Constructors ------------------------------ */
/* ------------------------------ Constructors ------------------------------ */
/* ------------------------------ Constructors ------------------------------ */
/* ------------------------------ Constructors ------------------------------ */
Server_launch::Server_launch(std::vector<Server> *__vecs) : __server_list(__vecs)
{	
	//create a new kernel event queue
	__kq = kqueue();
	if (__kq < 0)
	{
		std::cerr << "Failed to creating the kernel event queue" << std::endl;
		exit (1);
	}
}

Server_launch::~Server_launch()
{}

/* ------------------------------- Extraction ------------------------------- */
/* ------------------------------- Extraction ------------------------------- */
/* ------------------------------- Extraction ------------------------------- */
/* ------------------------------- Extraction ------------------------------- */
void Server_launch::__extraction(std::string &host_port, socket_info &__i)
{
	//todo: extrair port and ip@ + check if they are valide

	size_t	__p;
	if ((__p = host_port.find(':')) != std::string::npos)
	{
		__i.__host = host_port;
		__i.__address = host_port.substr(0, __p);
		__i.__port = stoi(host_port.substr(__p + 1));
	}
}


/* --------------------------------- Socket --------------------------------- */
/* --------------------------------- Socket --------------------------------- */
/* --------------------------------- Socket --------------------------------- */
/* --------------------------------- Socket --------------------------------- */
void Server_launch::__create_socket(socket_info &socket_add)
{
	socket_add.__socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_add.__socket_fd < 0)
	{
		std::cerr << "Failed to create socket" << std::endl;
		exit (1);
	}
}

/* ------------------------------- setsockopt ------------------------------- */
/* ------------------------------- setsockopt ------------------------------- */
/* ------------------------------- setsockopt ------------------------------- */
/* ------------------------------- setsockopt ------------------------------- */
void Server_launch::__set_socket(socket_info &socket_add)
{
	int	opt = 1;
	if(setsockopt(socket_add.__socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 ||
	setsockopt(socket_add.__socket_fd, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) < 0)
	{
		std::cerr << "Failed to set socket" << std::endl;
		exit (1);
	}
}

/* ---------------------------------- fcntl --------------------------------- */
/* ---------------------------------- fcntl --------------------------------- */
/* ---------------------------------- fcntl --------------------------------- */
/* ---------------------------------- fcntl --------------------------------- */
void Server_launch::__fcntl(socket_info &socket_add)
{
	if (fcntl(socket_add.__socket_fd, F_SETFL,  O_NONBLOCK) < 0)
	{
		std::cerr << "Error: fcntl" << std::endl;
		exit (1);
	}
}

/* ---------------------------------- bind ---------------------------------- */
/* ---------------------------------- bind ---------------------------------- */
/* ---------------------------------- bind ---------------------------------- */
/* ---------------------------------- bind ---------------------------------- */
void Server_launch::__bind_socket(socket_info &socket_add)
{
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(socket_add.__address.c_str());
	server_address.sin_port = htons(socket_add.__port);

	if (bind(socket_add.__socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
	{
		std::cerr << "Error: bind" << std::endl;
		exit (1);
	}		
}

/* --------------------------------- listen --------------------------------- */
/* --------------------------------- listen --------------------------------- */
/* --------------------------------- listen --------------------------------- */
/* --------------------------------- listen --------------------------------- */
void Server_launch::__listen_socket(socket_info &socket_add)
{
	if (listen(socket_add.__socket_fd, LISTEN_BACKLOG) < 0 )
	{
		std::cerr << "Error: listen" << std::endl;
		exit (1);
	}
}

/* --------------------------------- accept --------------------------------- */
/* --------------------------------- accept --------------------------------- */
/* --------------------------------- accept --------------------------------- */
/* --------------------------------- accept --------------------------------- */
void Server_launch::__accept(int __ident)
{
    struct kevent event;
    
    int __client = accept(__ident, NULL, NULL);
    if (__client < 0)
	{ 
		std::cerr << "Error: accept" << std::endl;
		exit (1);
	}

    EV_SET(&event, __client, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, &__read_handler[__client]);
	__read_handler[__client].__ident = __ident;
	__read_handler[__client].__scoket = __client;
	// __read_handler[__client].__server = __globle_sockets[__ident].__server;
    kevent(__kq, &event, 1, NULL, 0, 0);
}

/* ------------------------- kernel queue monitoring ------------------------ */
/* ------------------------- kernel queue monitoring ------------------------ */
/* ------------------------- kernel queue monitoring ------------------------ */
/* ------------------------- kernel queue monitoring ------------------------ */
void Server_launch::__kernel_event_queue()
{
	__itr_globle_sockets __it = __globle_sockets.begin();

	for (; __it != __globle_sockets.end(); ++__it)
	{
		struct kevent __k = {};
		__in_events.push_back(__k);
		EV_SET(&__in_events.back(), __it->first, EVFILT_READ, EV_ADD , 0, 0, NULL);
	}
	kevent(__kq, __in_events.data(), static_cast<int>(__in_events.size()), NULL, 0, 0);
}

/* ------------------------------- host exist ------------------------------- */
/* ------------------------------- host exist ------------------------------- */
/* ------------------------------- host exist ------------------------------- */
/* ------------------------------- host exist ------------------------------- */
int Server_launch::__host_exist(socket_info &__info)
{
	__itr_globle_sockets	__it = __globle_sockets.begin();

	while (__it != __globle_sockets.end())
	{
		if (__it->second.__address == __info.__address && __it->second.__port == __info.__port)
			break;
		++__it;
	}
	
	if(__it == __globle_sockets.end())
		return -1;
	return __it->first;
}


/* -------------------------------- Lunching -------------------------------- */
/* -------------------------------- Lunching -------------------------------- */
/* -------------------------------- Lunching -------------------------------- */
/* -------------------------------- Lunching -------------------------------- */
void Server_launch::__launch()
{
	std::vector<std::string>								*__host_port;
	int														__fd;

	for (size_t	i = 0; i < __server_list->size(); ++i)
	{
		__host_port = &(*__server_list)[i].__attributes["listen"];
		for (size_t __p = 0; __p < __host_port->size(); ++__p)
		{
			socket_info													__info;
			__extraction((*__host_port)[__p], __info);
			__fd = __host_exist(__info);
			if (__fd == -1)
			{
				__create_socket(__info);
				__set_socket(__info);
				__fcntl(__info);
				__bind_socket(__info);
				__listen_socket(__info);
				__info.__servers.push_back(&(*__server_list)[i]);
				__globle_sockets[__info.__socket_fd] = __info;
			}
			else
				__globle_sockets[__fd].__servers.push_back(&(*__server_list)[i]);
		}
	}
	__kernel_event_queue();
}

/* --------------------------------- Runing --------------------------------- */
/* --------------------------------- Runing --------------------------------- */
/* --------------------------------- Runing --------------------------------- */
/* --------------------------------- Runing --------------------------------- */
void Server_launch::__run()
{
	int			__event_number;

	__out_events.resize(EVENTS_NUBMBER);
	while (true)
	{
		if ((__event_number =  kevent(__kq, NULL, 0, __out_events.data(), EVENTS_NUBMBER, 0)) < 0)
		{
			std::cerr << "Error: kevent out" << std::endl;
			exit(1);
		}
		for (size_t i = 0; i < static_cast<size_t>(__event_number); ++i)
		{
			int		__ident = static_cast<int>(__out_events[i].ident);
			int		__data = static_cast<int>(__out_events[i].data);
			void*	__handler = __out_events[i].udata;
			if (__out_events[i].filter == EVFILT_READ)
			{
				if (__handler)
				{
					__input_handler(__ident, __data, static_cast<Request*>(__handler));
				}
				else
				{
					__accept(__ident);
				}
			}
			else if (__out_events[i].filter == EVFILT_WRITE)
			{
				std::cout << "EVFILT_WRITE" << std::endl;
				// send response
				if (__read_handler[__ident].__close)
					close(__ident);
				else
				{
					struct kevent event;
					EV_SET(&event, __ident, EVFILT_READ,  EV_ADD | EV_CLEAR, 0, 0, &__read_handler[__ident]);
				}
			}
		}
	}
}

Server *Server_launch::__server_set(int __ident, std::string &__host)
{
	std::vector<std::string>			*__v;
	size_t								__i = 0;

	while (__i < __globle_sockets[__ident].__servers.size())
	{
		__v = &(__globle_sockets[__ident].__servers[__i]->__attributes["server_name"]);
		if(__v->size() > 0 && std::find(__v->begin(), __v->end(), __host) != __v->end())
		{
			return __globle_sockets[__ident].__servers[__i];
		}
		__i++;
	}
	return __globle_sockets[__ident].__servers[0];
}

void Server_launch::__input_handler(int __ident, int __data, Request *__r)
{
	__r->__request_read(__ident, __data);
	__r->__server = __server_set(__r->__ident, __r->__host);
	std::cout << __r->__server->__attributes["root"].front() << std::endl;
	if (__r->__body_read_done || __r->__content_length == 0)
	{
		struct kevent event;

		// delete event
		EV_SET(&event, __ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		kevent(__kq, &event, 1, NULL, 0, 0);

		// add write event
		EV_SET(&event, __ident, EVFILT_WRITE,  EV_ADD | EV_CLEAR, 0, 0, NULL);
		kevent(__kq, &event, 1, NULL, 0, 0);

		// std::cout << __read_handler[__ident].__head << std::endl;
		// std::cout << __read_handler[__ident].__body << std::endl;

		/* --------------------------- parse requset ---------------------------- */
		//TODO:

		/* ------------------------------ execute ------------------------------- */
		//TODO:

        if (__r->__close)
			__read_handler.erase(__r->__scoket);
        else
			__r->__init_requst();

	}
}