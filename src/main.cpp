#include "net/TCPServer.h"
#include "fix/FixParser.h"
#include "fix/FixSessionManager.h"

#include <spdlog/spdlog.h>


int main() {
    TCPServer server(1);
    if (!server.start(5000)) {
        spdlog::error("Could not start TCP server");
        return 1;
    }
    spdlog::info("Server started on port 5000");

    if (server.acceptClient() == -1) {
        spdlog::error("Failed to accept client");
        return 1;
    }
    spdlog::info("Client accepted");

    const char delimiter { '|' };

    FixParser parser(delimiter);
    FixSessionManager session(server, parser, delimiter);
    session.run();

    server.stop();
    return 0;
}
