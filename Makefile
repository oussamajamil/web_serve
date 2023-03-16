# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oussama <oussama@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/26 16:38:20 by obelkhad          #+#    #+#              #
#    Updated: 2023/03/14 16:07:24 by oussama          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRC	=  main.cpp src/server.cpp\
				\
				utils/utl_string.cpp\
				utils/utl_helper.cpp\
				\
				utils.cpp\
				request/request.cpp\
				cerr/errors.cpp

OBJ = $(SRC:.cpp=.o)

NAME = webserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o:%.cpp include/*.hpp utils.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ)

fclean: clean 
	rm -f $(NAME)

re: fclean all