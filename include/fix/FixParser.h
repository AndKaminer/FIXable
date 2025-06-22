#ifndef INCLUDE_FIX_FIXPARSER_H_
#define INCLUDE_FIX_FIXPARSER_H_

#include <spdlog/spdlog.h>

#include <optional>
#include <sstream>
#include <string>

#include "fix/FixMessage.h"

class FixParser {
 private:
  char delimiter;
  bool parseToken(const std::string& token, FixMessage* fm);

 public:
  explicit FixParser(const char delimiter);
  std::optional<FixMessage> parseMessage(const std::string& inputString);
  char getDelimiter() const;
};

#endif  // INCLUDE_FIX_FIXPARSER_H_
