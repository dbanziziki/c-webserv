#include "server.hpp"

int main(int argc, char const *argv[]) {
    Server webserv(8080, 10);

    webserv.runServer();
    return 0;
}
