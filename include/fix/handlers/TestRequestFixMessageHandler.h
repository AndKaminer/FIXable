#ifndef INCLUDE_FIX_TESTREQUESTFIXMESSAGEHANDLER_H_
#define INCLUDE_FIX_TESTREQUESTFIXMESSAGEHANDLER_H_

#include <spdlog/spdlog.h>

#include <optional>
#include <string>

#include "fix/FixMessage.h"
#include "fix/FixSessionManager.h"
#include "fix/handlers/FixMessageHandler.h"

class TestRequestFixMessageHandler : public FixMessageHandler {
 public:
  virtual void handle(const FixMessage& msg,
                      FixSessionManager* sessionManager) override;
};

#endif  // INCLUDE_FIX_TESTREQUESTFIXMESSAGEHANDLER_H_
