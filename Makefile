# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obelkhad <obelkhad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/26 16:38:20 by obelkhad          #+#    #+#              #
#    Updated: 2023/02/28 12:19:14 by obelkhad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC	=  main.cpp src/server.cpp\
				utils/utl_string.cpp\
				cerr/errors.cpp

OBJ = $(SRC:.cpp=.o)

NAME = webserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o:%.cpp include/*.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ)

fclean: clean 
	rm -f $(NAME)

re: fclean all