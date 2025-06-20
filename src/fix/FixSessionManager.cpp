#include "fix/FixSessionManager.h"

FixSessionManager::FixSessionManager(TCPServer& server, FixParser& parser, const char delimiter)
    : server(server), parser(parser), delimiter(delimiter), buffer("") {}

void FixSessionManager::handleFixMessage(const FixMessage& msg) {
    auto type = msg.get(35);
    if (!type.has_value()) {
        spdlog::warn("Message missing 35 (MsgType)");
        return;
    }

    spdlog::info("Received FIX message type {}:\n{}", *type, msg.toStringHR());
    server.writeToClient(msg.toString());
}

bool FixSessionManager::tryExtractFixMessage(std::string& messageOut) {
    const std::string checksumTag = "10=";
    size_t start = 0;
    size_t tag10Pos = buffer.find(checksumTag, start);
    
    if (tag10Pos == std::string::npos) {
        return false;
    }
    
    size_t checksumEnd = buffer.find(delimiter, tag10Pos);
    if (checksumEnd == std::string::npos) {
        return false;
    }

    messageOut = buffer.substr(0, checksumEnd + 1);
    buffer.erase(0, checksumEnd + 1);
    return true;
}

void FixSessionManager::run() {
    while (server.isClientConnected()) {
        std::string data = server.readFromClient();
        if (data.empty()) break;

        buffer += data;

        std::string fixRaw;
        while (tryExtractFixMessage(fixRaw)) {
            std::optional<FixMessage> parsed = parser.parseMessage(fixRaw);
            if (parsed.has_value()) {
                handleFixMessage(parsed.value());
            }
            else {
                spdlog::error("Failed to log FIX message:\n{}", fixRaw);
            }
        server.writeToClient(fixRaw);
        }
    }
}
