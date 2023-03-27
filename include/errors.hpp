/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oussama <oussama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 12:16:56 by obelkhad          #+#    #+#             */
/*   Updated: 2023/03/14 15:46:54 by oussama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <iostream>
#include <fstream>

void __unclosed_curly();
void __curly_right_check(std::string __curly);
void __file_fail(std::ifstream &__file);
void __extention_is_good(std::string str);
void __bad_number_arguments();
void __semi_colon_missing();
void __attributes_missing();
void __bad_syntax();

void __wrong_location_attribute();