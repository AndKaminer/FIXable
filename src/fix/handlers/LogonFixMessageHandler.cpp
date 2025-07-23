#include "fix/handlers/LogonFixMessageHandler.h"

void LogonFixMessageHandler::handle(const FixMessage& msg,
                                    FixSessionManager* sessionManager) {
  sessionManager->handleLogon(msg);
}
