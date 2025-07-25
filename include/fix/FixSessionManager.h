#ifndef INCLUDE_FIX_FIXSESSIONMANAGER_H_
#define INCLUDE_FIX_FIXSESSIONMANAGER_H_

#include <spdlog/spdlog.h>

#include <chrono>
#include <iomanip>
#include <optional>
#include <string>

#include "fix/FixEncoder.h"
#include "fix/FixMessage.h"
#include "fix/FixMessageDispatcher.h"
#include "fix/FixParser.h"
#include "fix/FixSessionState.h"
#include "fix/handlers/LogonFixMessageHandler.h"
#include "fix/handlers/TestRequestFixMessageHandler.h"
#include "net/TCPServer.h"

class FixSessionManager {
 public:
  FixSessionManager(TCPServer* server, FixParser* parser, const char delimiter);
  void run();
  bool sendMessage(const FixMessage& msg);
  bool isLoggedOn();
  void handleLogon(const FixMessage& msg);
  int getNextOutgoingSeqNum();
  std::string getTargetCompID() const;
  std::string getSenderCompID() const;

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
  std::string getUTCTimestamp();
};

#endif  // INCLUDE_FIX_FIXSESSIONMANAGER_H_
