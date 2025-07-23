#ifndef INCLUDE_FIX_LOGONFIXMESSAGEHANDLER_H_
#define INCLUDE_FIX_LOGONFIXMESSAGEHANDLER_H_

#include "fix/FixMessage.h"
#include "fix/handlers/FixMessageHandler.h"
#include "fix/FixSessionManager.h"

#include <spdlog/spdlog.h>

#include <optional>
#include <string>

class LogonFixMessageHandler : public FixMessageHandler {
 public:
  virtual void handle(const FixMessage& msg, FixSessionManager* sessionManager) override;
};

#endif  // INCLUDE_FIX_LOGONFIXMESSAGEHANDLER_H_
