# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/26 16:38:20 by obelkhad          #+#    #+#              #
#    Updated: 2023/04/09 23:49:46 by obelkhad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++ -std=c++98

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRC	=  main.cpp src/server.cpp\
				src/server_launch.cpp\
				src/transfer.cpp\
				src/cgi.cpp\
				\
				utils/utl_string.cpp\
				utils/utl_helper.cpp\
				utils/utl_server.cpp\
				\
				utils.cpp\
				src/request.cpp\
				src/response.cpp\
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