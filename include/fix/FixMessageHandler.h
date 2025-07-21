#ifndef INCLUDE_FIX_FIXMESSAGEHANDLER_H_
#define INCLUDE_FIX_FIXMESSAGEHANDLER_H_

#include "fix/FixMessage.h"
#include "fix/FixSessionManager.h"

class FixSessionManager;

class FixMessageHandler {
 public:
  virtual ~FixMessageHandler() = default;
  virtual void handle(const FixMessage& msg, FixSessionManager* sessionManager) = 0;
};

#endif  // INCLUDE_FIX_FIXMESSAGEHANDLER_H_
