#include "fix/FixParser.h"


bool isConvertibleToInt(const std::string& str) {
    try {
       std::stoi(str);
       return true;
    } catch (const std::invalid_argument& e) {
       return false;
    } catch (const std::out_of_range& e) {
       return false;
    }
}

FixParser::FixParser(const char delimiter) {
    this -> delimiter = delimiter;
}

char FixParser::getDelimiter() const {
    return delimiter;
}

std::optional<FixMessage> FixParser::parseMessage(
        const std::string& inputString) {
    FixMessage fm = FixMessage();
    std::string token;
    std::stringstream ss(inputString);
    while (std::getline(ss, token, delimiter)) {
        bool success = parseToken(token, &fm);
        if (!success) {
            spdlog::error("Failed parsing token {}", token);
            return std::nullopt;
        }
    }

    if (!fm.isValid()) return std::nullopt;

    return fm;
}

bool FixParser::parseToken(const std::string& token, FixMessage* fm) {
    auto pos = token.find('=');
    if (pos == std::string::npos) return false;

    std::string tagStr = token.substr(0, pos);
    std::string value = token.substr(pos + 1);

    if (!isConvertibleToInt(tagStr)) return false;
    int tag = std::stoi(tagStr);

    fm->addField(tag, value);
    return true;
}
