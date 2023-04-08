/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sender.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 09:46:25 by obelkhad          #+#    #+#             */
/*   Updated: 2023/04/08 10:26:53 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SENDER_HPP
#define SENDER_HPP

#include <string>

struct Sender
{
public:
	std::string						__response;

	Sender();
	~Sender();

	void							__response_send(int __ident, int __data);
};

#endif