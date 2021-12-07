#include "Client.hpp"

Client::Client(int port) : port(port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    socketConnect();
    std::cout << "Connected to the server" << std::endl;
    memset(serverReply, 0, 200);
}

Client::~Client() {}

int Client::socketConnect() {
    int iRetVal = -1;

    struct sockaddr_in remote = {0};
    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); /* local host */
    remote.sin_family = AF_INET;
    remote.sin_port = htons(port);
    iRetVal =
        connect(sock, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));
    return iRetVal;
}

int Client::socketSend(char *rqst, short lenRqst) {
    int shortRetVal = -1;

    shortRetVal = send(sock, rqst, lenRqst, 0);
    return shortRetVal;
}

int Client::socketReceive(char *rsp, short rcvSize) {
    int shortRetVal = -1;

    shortRetVal = recv(sock, rsp, rcvSize, 0);
    return shortRetVal;
}

void Client::runClient() {
    while (true) {
        std::cout << "Enter the message: ";
        std::cin >> sendToServer;
        socketSend((char *)sendToServer.data(), sendToServer.length());
        memset(serverReply, 0, 200);
        readSize = socketReceive(serverReply, 200);
        std::cout << "Message received: " << serverReply << std::endl;
    }
}