#ifndef FIXSESSIONMANAGER_H
#define FIXSESSIONMANAGER_H

#include "net/TCPServer.h"
#include "fix/FixParser.h"
#include "fix/FixMessage.h"

#include <string>
#include <optional>

#include <spdlog/spdlog.h>

class FixSessionManager {
public:
    FixSessionManager(TCPServer& server, FixParser& parser, const char delimiter);
    void run();

private:
    TCPServer& server;
    FixParser& parser;
    std::string buffer;
    const char delimiter;

    void handleFixMessage(const FixMessage& msg);
    bool tryExtractFixMessage(std::string& messageOut);
};

#endif
