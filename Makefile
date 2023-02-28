COPILER = c++
CFLAGS = -Wall -g -std=c++98
FILES = server.cpp\
			request.cpp\
			utils.cpp
HEADERS = request.hpp\
			utils.hpp


all: main 

main: $(FILES) $(HEADERS)
	$(COPILER) $(CFLAGS) $(FILES) -o server


clean:
	rm -f server



