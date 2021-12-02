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

char **ft_split(char const *s, char c);
char BUFFER[4096];

void free_tab(char **tab, int from) {
    int i = from;

    while (tab[++i]) {
        free(tab[i]);
    }
    free(tab);
    tab = NULL;
}

const int PORT = 8080;

short socketCreate(void) {
    short hsocket = 0;
    hsocket = socket(AF_INET, SOCK_STREAM, 0);
    return hsocket;
}

int bindCreateScoket(int hSocket) {
    int iRetVal = -1;
    struct sockaddr_in remote = {0};

    /* internet address family */
    remote.sin_family = AF_INET;

    /* any incoming interface */
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(PORT); /* local port */
    iRetVal = bind(hSocket, (struct sockaddr *)&remote, sizeof(remote));
    return iRetVal;
}

int main(int argc, char const *argv[]) {
    /* code */

    int sock_desc = 0, sock = 0, clientLen = 0;

    struct sockaddr_in client;
    char client_message[200] = {0};
    char message[100] = {0};
    const char *pMessage = "Hello there David";

    sock_desc = socketCreate();
    if (sock_desc == -1) {
        printf("failed to create the server\n");
        return 1;
    }
    printf("socket created\n");
    if (bindCreateScoket(sock_desc) < 0) {
        perror("bind failed");
    }
    printf("bind is done\n");

    listen(sock_desc, 3);
    while (1) {
        printf("waiting for incoming contections...\n");
        clientLen = sizeof(struct sockaddr_in);
        sock = accept(sock_desc, (struct sockaddr *)&client,
                      (socklen_t *)&clientLen);
        if (sock < 0) {
            perror("accept failed");
            return 1;
        }
        printf("Connection accepted\n");
        memset(client_message, 0, sizeof(client_message));
        memset(message, 0, sizeof(message));

        /* Receive a reply from the the first client */
        if (recv(sock, client_message, 200, 0) < 0) {
            printf("recv failed\n");
            break;
        }
        // /* close the first client*/
        // int i = atoi(client_message);
        // i--;
        // sprintf(message, "%d", i);
        // close(sock);
        // printf("waiting for the second client...\n");
        // sock =
        //     accept(sock_desc, (struct sockaddr *)&client, (socklen_t
        //     *)&clientLen);

        strcpy(message,
               "HTTP/1.1 200 OK\nContent-Type: "
               "text/html; charset=UTF-8\nContent-Length: 21\n\n<h1>Hello "
               "there!</h1>");
        /* send some data to the second client */
        if (send(sock, message, strlen(message), 0) < 0) {
            printf("Send failed");
            return 1;
        }
        close(sock);
    }
    return 0;
}
