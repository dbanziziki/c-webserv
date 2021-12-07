#include "Client.hpp"

int main(int argc, char const *argv[]) {
    Client client(8080);

    client.runClient();
    return 0;
}
