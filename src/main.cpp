#include "net/TCPServer.h"

#include <unistd.h>
#include <iostream>

int main() {
    TCPServer server = TCPServer(1);
    server.start(8080);
    server.run();
    server.stop();
}
