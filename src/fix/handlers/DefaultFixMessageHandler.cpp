#include "fix/handlers/DefaultFixMessageHandler.h"

void DefaultFixMessageHandler::handle(const FixMessage& msg, FixSessionManager* sessionManager) {
  sessionManager->sendMessage(msg);
}
