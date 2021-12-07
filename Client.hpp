#if !defined(__CLIENT_H__)
#define __CLIENT_H__

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

class Client {
   private:
    short sock;
    int port;
    struct sockaddr_in server;
    std::string sendToServer;
    char serverReply[200];
    int readSize;

   public:
    Client(/* args */){};
    Client(int port);
    ~Client();

    void runClient();

   private:
    int socketConnect();
    int socketSend(char *rqt, short lenRqst);
    int socketReceive(char *rsp, short rcvSize);
};

#endif  // __CLIENT_H__
