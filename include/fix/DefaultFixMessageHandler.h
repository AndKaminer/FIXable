#ifndef INCLUDE_FIX_DEFAULTFIXMESSAGEHANDLER_H_
#define INCLUDE_FIX_DEFAULTFIXMESSAGEHANDLER_H_

#include "fix/FixMessage.h"
#include "fix/FixMessageHandler.h"
#include "fix/FixSessionManager.h"
#include "fix/FixEncoder.h"

#include <spdlog/spdlog.h>

#include <optional>
#include <string>

class DefaultFixMessageHandler : public FixMessageHandler {
 public:
  virtual void handle(const FixMessage& msg, FixSessionManager* sessionManager) override;
};

#endif  // INCLUDE_FIX_DEFAULTFIXMESSAGEHANDLER_H
