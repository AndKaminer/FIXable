#ifndef INCLUDE_FIX_FIXSESSIONMANAGER_H_
#define INCLUDE_FIX_FIXSESSIONMANAGER_H_

#include <spdlog/spdlog.h>

#include <optional>
#include <string>

#include "fix/handlers/LogonFixMessageHandler.h"
#include "fix/FixEncoder.h"
#include "fix/FixMessage.h"
#include "fix/FixMessageDispatcher.h"
#include "fix/FixParser.h"
#include "fix/FixSessionState.h"
#include "net/TCPServer.h"

class FixSessionManager {
 public:
  FixSessionManager(TCPServer* server, FixParser* parser, const char delimiter);
  void run();
  bool sendMessage(const FixMessage& msg);
  bool isLoggedOn();
  void handleLogon(const FixMessage& msg);

 private:
  TCPServer* server;
  FixParser* parser;
  std::string buffer;
  const char delimiter;
  FixSessionState session;
  FixMessageDispatcher dispatcher;

  void handleFixMessage(const FixMessage& msg);
  bool tryExtractFixMessage(std::string* messageOut);
  void registerHandlers();
};

#endif  // INCLUDE_FIX_FIXSESSIONMANAGER_H_
