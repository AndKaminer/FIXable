#ifndef FIXPARSER_H
#define FIXPARSER_H

#include "fix/FixMessage.h"

#include <spdlog/spdlog.h>

#include <string>
#include <optional>
#include <sstream>

class FixParser {
private:
    char delimiter;
    bool parseToken(const std::string& token, FixMessage& fm);

public:
    FixParser(const char delimiter);
    std::optional<FixMessage> parseMessage(const std::string& inputString);
    char getDelimiter() const;
};

#endif
