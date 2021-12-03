#if !defined(_SERVER_H_)
#define _SERVER_H_

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

class Server {
   private:
    int sock;
    int port;
    // std::string clientRequest;
    // std::string serverResponse;
    char clientRequest[200];
    char serverResponse[200];
    struct sockaddr_in client;
    int clientLen;

   public:
    Server(/* args */) {}
    Server(int port, int max);
    ~Server();

    void runServer(void);

   private:
    int socketCreate(void);
    int bindCreateSocket(int sock);
};

#endif  // _SERVER_H_
