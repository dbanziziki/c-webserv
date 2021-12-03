#include "server.hpp"

Server::Server(int port, int max) : port(port) {
    clientLen = sizeof(struct sockaddr_in);
    sock = socketCreate();
    if (sock < 0) {
        // handle error
    }
    bindCreateSocket(sock);
    listen(sock, max);
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

void Server::runServer(void) {
    int clientSock = 0;
    struct pollfd pfds[5];
    int fd_count = 0;

    pfds[0].fd = sock;
    pfds[0].events = POLLIN;
    fd_count = 1;
    while (true) {
        std::cout << "Waiting for incoming connections..." << std::endl
                  << std::endl;
        int poll_count =
            poll(pfds, fd_count, -1);  // negative value to listen forever
        if (poll_count == -1) {
            std::cout << "poll() failed" << std::endl;
            exit(1);
        }

        // loop trough the existing connections looking for data to read
        for (int i = 0; i < fd_count; i++) {
            if (pfds[i].revents & POLLIN) {  // we got one
                if (pfds[i].fd == sock) {
                    // if out listener is ready to read, handle new connection
                    clientSock = accept(sock, (struct sockaddr *)&client,
                                        (socklen_t *)&clientLen); /* blocks */
                    if (clientSock < 0) {
                        std::cout << "Accepte failed" << std::endl;
                        // return; dont wanna quit?????
                    } else {
                        // add the new connection in the poll
                        pfds[fd_count].fd = clientSock;
                        pfds[fd_count].events = POLLIN;
                        fd_count++;
                        std::cout << "New connection added to the poll"
                                  << std::endl;
                    }
                } else {
                    // if it's not out listener, it's just a client
                    std::cout << "Client ready" << std::endl;
                    memset(clientRequest, 0, 200);
                    int nbytes = recv(clientSock, clientRequest, 200, 0);
                    int sender_fd = pfds[i].fd;
                    if (nbytes <= 0) {
                        // error or connection closed by client
                        if (nbytes == 0) {
                            std::cout << "Server: socket " << sender_fd
                                      << " hung up\n"
                                      << std::endl;
                        } else {
                            std::cout << "recv() failed" << std::endl;
                        }
                        // copy the last one to pfds[i]
                        close(pfds[i].fd);
                        pfds[i] = pfds[fd_count - 1];
                        fd_count--;
                    } else {
                        std::cout << "Received data: " << clientRequest
                                  << " From: " << sender_fd << std::endl;
                        for (int j = 0; j < fd_count; j++) {
                            // send the data to everyone connected
                            int dest_fd = pfds[j].fd;

                            // Except the listener and ourselveso
                            if (dest_fd != sock) {
                                // memset(serverResponse, 0, 200);
                                if (send(clientSock, clientRequest, nbytes, 0) <
                                    0) {
                                    std::cout << "Send failed" << std::endl;
                                    // return;
                                }
                            }
                        }
                    }
                }
            }
        }

        // std::cout << clientRequest << std::endl;
        // strcpy(serverResponse,
        //        "HTTP/1.1 200 OK\nContent-Type: "
        //        "text/html; charset=UTF-8\nContent-Length: 21\n\n<h1>Hello "
        //        "there!</h1>");
        // close(clientSock);
    }
}