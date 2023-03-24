/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 18:17:04 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/24 14:48:07 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <map>
#include "../include/server.hpp"
#include "../include/receive.hpp"

typedef struct socket_info
{
	int						__port;
	int						__socket_fd;
	std::vector<Server*>	__servers;
	std::string				__address;
	std::string				__host;
}socket_info;

class Server_launch
{
private:
	int															__kq;
	std::vector<struct kevent>									__in_events;
	std::vector<struct kevent>									__out_events;
	std::vector<Server>											*__server_list;
	std::map<int, Receive>										__read_handler;
	typedef std::map<int, socket_info>::iterator				__itr_globle_sockets;
public:

	std::map<int, socket_info>									__globle_sockets;

	Server_launch(std::vector<Server> *);
	~Server_launch();

	Server*						__server_set(int __ident, std::string &);

	void 						__launch();
	void 						__run();
	int							__host_exist(socket_info &__info);
	void						__create_socket(socket_info &);
	void						__set_socket(socket_info &);
	void						__fcntl(socket_info &);
	void						__bind_socket(socket_info &);
	void						__listen_socket(socket_info &);
	void						__accept(int);
	void						__kernel_event_queue();
	void						__extraction(std::string &, socket_info &);
	void						__input_handler(int __ident, int __data, Receive *__r);
};
