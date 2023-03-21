/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:50:00 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/20 10:42:49 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../include/server.hpp"

/* utl_string.cpp */
std::string __config_file(int ac, char **av);
std::vector<std::string> __split_attrubites(std::string str, const std::string delims);
std::string __extract_parameters(std::string &line);
std::string __trim(std::string &expretion);
void __open_curly_bracket( bool __server, bool __loc_zone, bool __location);
void __close_curly_bracket( bool __server, bool __loc_zone, bool __location);
void __vector_display(std::vector<std::string> &vec);
void __map_display(std::map<std::string, std::vector<std::string> > &mp);
void __servers_display(Web &web);

/* utl_helper.cpp  */
void 	__address_prot_form(std::string &address);
size_t __crlf(std::string &str, size_t pos);