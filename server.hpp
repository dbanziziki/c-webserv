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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <streambuf>
#include <vector>

#include "HttpRequest.hpp"

class Server {
   private:
    int sock;
    int port;
    // std::string clientRequest;
    // std::string serverResponse;
    char clientRequest[1024];
    char serverResponse[1024];
    struct sockaddr_in client;
    int clientLen;
    fd_set master;
    bool close_conn;
    int maxSd;

   public:
    Server(/* args */) {}
    Server(int port, int max);
    ~Server();

    void runServer(void);

   private:
    int socketCreate(void);
    int acceptConnection();
    int onRequest();
    int sendResponse(int clientSock, char *request);
    int handleConnection(int clientSock);
    int bindCreateSocket(int sock);
};

#endif  // _SERVER_H_
