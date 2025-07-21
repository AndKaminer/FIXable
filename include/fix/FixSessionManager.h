#ifndef INCLUDE_FIX_FIXSESSIONMANAGER_H_
#define INCLUDE_FIX_FIXSESSIONMANAGER_H_

#include <spdlog/spdlog.h>

#include <optional>
#include <string>

#include "fix/DefaultFixMessageHandler.h"
#include "fix/FixMessage.h"
#include "fix/FixMessageHandler.h"
#include "fix/FixParser.h"
#include "net/TCPServer.h"

class FixSessionManager {
 public:
  FixSessionManager(TCPServer* server, FixParser* parser, const char delimiter);
  void run();
  bool sendMessage(const FixMessage& msg);

 private:
  TCPServer* server;
  FixParser* parser;
  std::string buffer;
  const char delimiter;

  void handleFixMessage(const FixMessage& msg);
  bool tryExtractFixMessage(std::string* messageOut);
};

#endif  // INCLUDE_FIX_FIXSESSIONMANAGER_H_
