#ifndef INCLUDE_FIX_DEFAULTFIXMESSAGEHANDLER_H_
#define INCLUDE_FIX_DEFAULTFIXMESSAGEHANDLER_H_

#include <spdlog/spdlog.h>

#include <optional>
#include <string>

#include "fix/FixEncoder.h"
#include "fix/FixMessage.h"
#include "fix/FixSessionManager.h"
#include "fix/handlers/FixMessageHandler.h"

class DefaultFixMessageHandler : public FixMessageHandler {
 public:
  virtual void handle(const FixMessage& msg,
                      FixSessionManager* sessionManager) override;
};

#endif  // INCLUDE_FIX_DEFAULTFIXMESSAGEHANDLER_H
