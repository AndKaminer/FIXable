#ifndef INCLUDE_FIX_LOGONFIXMESSAGEHANDLER_H_
#define INCLUDE_FIX_LOGONFIXMESSAGEHANDLER_H_

#include <spdlog/spdlog.h>

#include <optional>
#include <string>

#include "fix/FixMessage.h"
#include "fix/FixSessionManager.h"
#include "fix/handlers/FixMessageHandler.h"

class LogonFixMessageHandler : public FixMessageHandler {
 public:
  virtual void handle(const FixMessage& msg,
                      FixSessionManager* sessionManager) override;
};

#endif  // INCLUDE_FIX_LOGONFIXMESSAGEHANDLER_H_
