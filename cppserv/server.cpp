#include "server.hpp"

Server::Server(int port, int max) : port(port) {
    clientLen = sizeof(struct sockaddr_in);
    sock = socketCreate();
    if (sock < 0) {
        // handle error
    }
    fcntl(sock, F_SETFL, O_NONBLOCK);
    bindCreateSocket(sock);
    FD_ZERO(&master);
    FD_SET(sock, &master);
    listen(sock, max);
    // fcntl(sock, F_SETFL, O_NONBLOCK);
}

Server::~Server() {}

int Server::socketCreate(void) {
    short hsocket = 0;
    hsocket = socket(AF_INET, SOCK_STREAM, 0);
    return hsocket;
}

int Server::bindCreateSocket(int sock) {
    int iRetVal = -1;
    struct sockaddr_in remote = {0};

    /* internet address family */
    remote.sin_family = AF_INET;

    /* any incoming interface */
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(port); /* local port */
    iRetVal = bind(sock, (struct sockaddr *)&remote, sizeof(remote));
    return iRetVal;
}

int Server::receiveData(int clientSock) {
    memset(clientRequest, 0, 200);
    int nbytes =  recv(clientSock, clientRequest, 200, 0);
    if (nbytes <= 0) {
        std::cout << "Nothing recieved" << std::endl;
        close(clientSock);
        return -2;
    }
    std::string msg(clientRequest);
    if (msg.find("sleep") != std::string::npos) {
        std::cout << "sleeping request" << std::endl;
        sleep(5);
    }
    sendData(clientSock);
    std::cout << "Data sent" << std::endl;
    std::cout << clientRequest << std::endl;
    return 0;
}

int Server::sendData(int clientSock) {
    memset(serverResponse, 0, 200);
    strcpy(serverResponse,
           "HTTP/1.1 200 OK\nContent-Type: "
           "text/html; charset=UTF-8\nContent-Length: 21\n\n<h1>Hello "
           "there!</h1>");
    if (send(clientSock, serverResponse, strlen(serverResponse), 0) < 0) {
        std::cout << "Send failed" << std::endl;
        return -1;
    }
    return 0;
}

void Server::runServer(void) {
    int clientSock = 0;
    while (true) {
        std::cout << "Waiting for incoming connections..." << std::endl
                  << std::endl;
        clientSock = accept(sock, (struct sockaddr *)&client,
                            (socklen_t *)&clientLen); /* blocks */
        if (clientSock < 0) {
            std::cout << "Accepte failed" << std::endl;
            return;
        }
        std::cout << "Connection accepted" << std::endl;
        memset(clientRequest, 0, 200);
        memset(serverResponse, 0, 200);

        if (recv(clientSock, clientRequest, 200, 0) < 0) {
            std::cout << "recv failed" << std::endl;
            return;
        }
        std::cout << clientRequest << std::endl;
        strcpy(serverResponse,
               "HTTP/1.1 200 OK\nContent-Type: "
               "text/html; charset=UTF-8\nContent-Length: 21\n\n<h1>Hello "
               "there!</h1>");
        if (send(clientSock, serverResponse, strlen(serverResponse), 0) < 0) {
            std::cout << "Send failed" << std::endl;
            return;
        }
        close(clientSock);
    }
}