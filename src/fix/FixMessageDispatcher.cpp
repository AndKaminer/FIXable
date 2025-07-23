#include "fix/FixMessageDispatcher.h"

void FixMessageDispatcher::registerHandler(const std::string& msgType, std::unique_ptr<FixMessageHandler> h) {
  handlers[msgType] = std::move(h);
}

void FixMessageDispatcher::dispatch(const FixMessage& msg, FixSessionManager* session) const {
  auto msgType = msg.get(35);
  if (!msgType.has_value()) {
    spdlog::warn("Message missing 35 (MsgType)");
    return;
  }
  spdlog::info("Received FIX message msgType {}:\n{}", *msgType, msg.toStringHR());
  FixMessageHandler* handler;
  auto it = handlers.find(*msgType);
  if (it != handlers.end()) {
    it->second->handle(msg, session);
  } else {
    spdlog::warn("Unhandled message msgType: {}", *msgType);
  }
}
