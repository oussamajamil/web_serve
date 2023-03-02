CONETENT= request.cpp\
					server.cpp\
					../utils.cpp

NAME = parserRequest
HEADER = request.hpp\
			../utils.hpp
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98


all: $(NAME)

$(NAME): $(CONETENT) $(HEADER)
	$(CC) $(CFLAGS) $(CONETENT) -o $(NAME)


clean:
	rm -f $(NAME)

fclean: clean



