#include "net/TCPClient.h"
#include "fix/FixEncoder.h"
#include "fix/FixMessage.h"

#include <spdlog/spdlog.h>

#include <string>

int main() {
    TCPClient client(1);
    if (!client.connectClient("127.0.0.1", 5000)) {
        return 1;
    }

    FixMessage fm;
    fm.addField(35, "A");
    fm.addField(12, "Hello!");
    fm.addField(25, "Goodbye!");

    std::string message = FixEncoder().encode(fm, "", '\x01');
    client.sendMessage(message);
    spdlog::info(client.receiveMessage());

    client.disconnectClient();
}
