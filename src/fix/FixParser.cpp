#include "fix/FixParser.h"


bool isConvertibleToInt(const std::string& str) {
   try {
       std::stoi(str);
       return true;  // Conversion successful
   } catch (const std::invalid_argument& e) {
       return false; // Conversion failed (invalid format)
   } catch (const std::out_of_range& e) {
       return false; // Conversion failed (out of range)
   }
}

FixParser::FixParser(const char delimiter) {
    this -> delimiter = delimiter;
}

char FixParser::getDelimiter() const {
    return delimiter;
}

bool FixParser::isValid(const FixMessage& msg) {
    return msg.has(8) && msg.has(9) && msg.has(35) && msg.has(10);
}

std::optional<FixMessage> FixParser::parseMessage(const std::string& inputString) {
    FixMessage fm = FixMessage();
    std::string token;
    std::stringstream ss(inputString);
    while (std::getline(ss, token, delimiter)) {
        bool success = parseToken(token, fm);
        if (!success) {
            spdlog::error("Failed parsing token {}", token);
            return std::nullopt;
        }
    }

    if (!isValid(fm)) return std::nullopt;

    return fm;
}

bool FixParser::parseToken(const std::string& token, FixMessage& fm) {
    auto pos = token.find('=');
    if (pos == std::string::npos) return false;

    std::string tagStr = token.substr(0, pos);
    std::string value = token.substr(pos + 1);

    if (!isConvertibleToInt(tagStr)) return false;
    int tag = std::stoi(tagStr);

    fm.addField(tag, value);
    return true;
}
