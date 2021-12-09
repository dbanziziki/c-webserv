SERVER = server

CLIENT = client

all: server.cpp main.cpp HttpRequest.cpp
	clang++ -g -fsanitize=address server.cpp HttpRequest.cpp main.cpp -o $(SERVER)

fclean:
	rm -rf $(SERVER).dSYM $(CLIENT).dSYM
	rm -f $(CLIENT) $(SERVER)

re: fclean all