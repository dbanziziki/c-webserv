SERVER = server

CLIENT = client

all: server.cpp main.cpp client_main.cpp Client.cpp
	clang++ -g -fsanitize=address server.cpp main.cpp -o $(SERVER)
	clang++ -g Client.cpp client_main.cpp -o $(CLIENT)

fclean:
	rm -rf $(SERVER).dSYM $(CLIENT).dSYM
	rm -f $(CLIENT) $(SERVER)