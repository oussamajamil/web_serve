/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:50:00 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/01 11:18:59 by obelkhad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include <cstring>
// #include <vector>

#include <iostream>
#include <fstream>
#include <vector>

std::string __config_file(int ac, char **av);
std::vector<std::string> __split_attrubites(std::string str, const std::string delims);
std::string __extract_parameters(std::string &line);
std::string __trim(std::string &expretion);
void __vector_display(std::vector<std::string> &vec);
void __open_curly_bracket( bool __server, bool __loc_zone, bool __location);
void __close_curly_bracket( bool __server, bool __loc_zone, bool __location);