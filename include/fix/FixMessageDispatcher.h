#ifndef INCLUDE_FIX_FIXMESSAGEDISPATCHER_H_
#define INCLUDE_FIX_FIXMESSAGEDISPATCHER_H_

#include <unordered_map>
#include <memory>
#include <string>

#include <spdlog/spdlog.h>

#include "fix/FixMessage.h"
#include "fix/handlers/FixMessageHandler.h"

class FixSessionManager;

class FixMessageDispatcher {
public:
  void registerHandler(const std::string& msgType,
                       std::unique_ptr<FixMessageHandler> handler);

  void dispatch(const FixMessage& msg, FixSessionManager* session) const;

private:
  std::unordered_map<std::string, std::unique_ptr<FixMessageHandler>> handlers;
};

#endif  // INCLUDE_FIX_FIXMESSAGEDISPATCHER_H_
