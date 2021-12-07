#include "server.hpp"

#include <fstream>
#include <sstream>
#include <streambuf>
#include <vector>

Server::Server(int port, int max) : port(port), close_conn(false) {
    clientLen = sizeof(struct sockaddr_in);
    sock = socketCreate();
    if (sock < 0) {
        // handle error
    }
    maxSd = sock;
    fcntl(sock, F_SETFL, O_NONBLOCK);
    bindCreateSocket(sock);
    listen(sock, max);
    FD_ZERO(&master);
    FD_SET(sock, &master);
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

int Server::acceptConnection() {
    int clientSock = accept(sock, (struct sockaddr *)&client,
                            (socklen_t *)&clientLen); /* blocks */
    if (clientSock < 0) {
        std::cout << "Accepte failed" << std::endl;
        return -1;
    }
    std::cout << "Connection accepted" << std::endl;
    return clientSock;
}

int Server::sendResponse(int clientSock, char *request) {
    std::istringstream iss(request);

    std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)),
                                    std::istream_iterator<std::string>());
    std::string content = "<h1>404 Not Found</h1>";
    int errorCode = 404;
    std::string page = "index.html";
    if (parsed.size() >= 3 && parsed[0] == "GET") {
        page = parsed[1];
        if (page == "/") {
            page = "index.html";
        } else {
            std::string temp = page;
            page = "." + temp;
            std::cout << "The page is " << page << std::endl;
        }
    } else {
        page = "404.hmtl";
    }
    std::ifstream f(page);
    if (f.good()) {
        std::string str((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());
        content = str;
        errorCode = 200;
    }
    f.close();
    std::ostringstream oss;
    oss << "HTTP/1.1 " << errorCode << " OK\n";
    oss << "Content-Type: text/html; charset=UTF-8\n";
    oss << "Content-Length: " << content.size() << "\n\n";
    oss << content;
    int ret = send(clientSock, oss.str().c_str(), oss.str().size() + 1, 0);
    return ret;
}

int Server::handleConnection(int clientSock) {
    std::ostringstream oss;
    int ret = 0;
    do {
        ret = recv(clientSock, clientRequest, 1024, 0);
        if (ret < 0) {
            if (errno != EWOULDBLOCK) {
                perror("recv failed");
                close_conn = true;
            }
            break;
        }
        if (ret == 0) {
            std::cout << "Connection closed" << std::endl;
            close_conn = true;
            break;
        }

        int len = ret;
        ret = sendResponse(clientSock, clientRequest);
        if (ret < 0) {
            perror("send failed");
            close_conn = true;
            break;
        }
        oss.clear();
    } while (true);
    return 0;
}

int Server::onRequest() {
    int clientSock = 0;

    do {
        clientSock = acceptConnection();
        if (clientSock < 0) {
            if (errno != EWOULDBLOCK) {
                perror(" accept() faild");
                exit(EXIT_FAILURE);
            }
            break;
        }
        FD_SET(clientSock, &master);
        if (clientSock > maxSd) maxSd = clientSock;
    } while (clientSock != -1);
    return clientSock;
}

void Server::runServer(void) {
    int clientSock = 0;
    fd_set copy;
    int descReady;
    int ret;
    while (true) {
        copy = master;
        std::cout << "Waiting for incoming connections..." << std::endl
                  << std::endl;
        if ((descReady = select(maxSd + 1, &copy, NULL, NULL, NULL)) < 0) {
            std::cerr << "select() failed" << std::endl;
        }
        for (int i = 0; i <= maxSd && descReady; i++) {
            if (FD_ISSET(i, &copy)) {
                descReady -= 1;
                if (i == sock) {
                    std::cout << "Listening sock ready" << std::endl;
                    onRequest();
                } else {
                    std::cout << "Descriptor " << i << " is readable"
                              << std::endl;
                    close_conn = false;
                    handleConnection(i);
                    if (close_conn) {
                        close(i);
                        FD_CLR(i, &master);
                        if (i == maxSd) {
                            while (FD_ISSET(maxSd, &master) == false)
                                maxSd -= 1;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i <= maxSd; i++) {
        if (FD_ISSET(i, &master)) {
            close(i);
        }
    }
}