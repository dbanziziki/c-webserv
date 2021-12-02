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

short socketCreate(void) {
  short hsocket = 0;
  hsocket = socket(AF_INET, SOCK_STREAM, 0);
  return hsocket;
}

int socketConnect(int hSocket) {
  int iRetVal = -1;
  int serverPort = 12345;

  struct sockaddr_in remote = {0};
  remote.sin_addr.s_addr = inet_addr("127.0.0.1"); /* local host */
  remote.sin_family = AF_INET;
  remote.sin_port = htons(serverPort);

  iRetVal =
      connect(hSocket, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));
  return iRetVal;
}

int socketSend(int hSocket, char *rqst, short lenRqst) {
  int shortRetVal = -1;
  struct timeval tv;
  tv.tv_sec = 20;
  tv.tv_usec = 0;

  if (setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) <
      0) {
    printf("Time out\n");
    return -1;
  }
  shortRetVal = send(hSocket, rqst, lenRqst, 0);
  return shortRetVal;
}

int socketReceive(int hSocket, char *rsp, short rcvSize) {
  int shortRetVal = -1;
  struct timeval tv;
  tv.tv_usec = 0;
  tv.tv_sec = 20;
  if (setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) <
      0) {
    printf("Time out\n");
    return -1;
  }
  shortRetVal = recv(hSocket, rsp, rcvSize, 0);
  return shortRetVal;
}

int main(int argc, char const *argv[]) {
  /* code */
  int hSocket = 0, read_size = 0;
  struct sockaddr_in server;
  char sendToServer[100] = {0};
  char serverReply[200] = {0};

  hSocket = socketCreate();
  if (hSocket == -1) {
    printf("could not create the socket\n");
    return 1;
  }

  if (socketConnect(hSocket) < 0) {
    printf("could not connect to the server\n");
    return 1;
  }
  printf("success\n");

  /* receive data from server */
  read_size = socketReceive(hSocket, serverReply, 200);
  printf("server response: %s\n\n", serverReply);
  close(hSocket);
  return 0;
}