#include "fix/FixSessionManager.h"

FixSessionManager::FixSessionManager(TCPServer* server, FixParser* parser,
                                     const char delimiter)
    : server(server), parser(parser), delimiter(delimiter), buffer("") {
  registerHandlers();
}

void FixSessionManager::handleFixMessage(const FixMessage& msg) {
  session.incomingSeqNum++;
  dispatcher.dispatch(msg, this);
}

bool FixSessionManager::tryExtractFixMessage(std::string* messageOut) {
  const std::string checksumTag = "10=";
  size_t start = 0;
  size_t tag10Pos = buffer.find(checksumTag, start);

  if (tag10Pos == std::string::npos) {
    return false;
  }

  size_t checksumEnd = buffer.find(delimiter, tag10Pos);
  if (checksumEnd == std::string::npos) {
    return false;
  }

  *messageOut = buffer.substr(0, checksumEnd + 1);
  buffer.erase(0, checksumEnd + 1);
  return true;
}

void FixSessionManager::run() {
  while (server->isClientConnected()) {
    std::string data = server->readFromClient();
    if (data.empty()) break;

    buffer += data;

    std::string fixRaw;
    while (tryExtractFixMessage(&fixRaw)) {
      std::optional<FixMessage> parsed = parser->parseMessage(fixRaw);
      if (parsed.has_value()) {
        handleFixMessage(parsed.value());
      } else {
        spdlog::error("Failed to log FIX message:\n{}", fixRaw);
      }
    }
  }
}

bool FixSessionManager::sendMessage(const FixMessage& msg) {
  if (!server->isClientConnected()) {
    spdlog::error("Client is not connected. Cannot send message.");
    return false;
  }
  std::optional<std::string> beginString = msg.get(8);
  if (!beginString.has_value()) {
    spdlog::warn("Message has no 8 tag. Cannot send message.");
    return false;
  }

  std::string msgText = FixEncoder::encode(msg, beginString.value());
  spdlog::info("Sending message {} to client", msgText);
  server->writeToClient(msgText);
  return true;
}

void FixSessionManager::handleLogon(const FixMessage& msg) {
  std::string sender = msg.get(49).value_or("");
  std::string target = msg.get(56).value_or("");

  session.senderCompID = sender;
  session.targetCompID = target;
  session.isLoggedOn = true;

  spdlog::info("Logon from {} -> {}", sender, target);

  FixMessage logonReply;
  logonReply.addField(35, "A");
  logonReply.addField(34, std::to_string(session.outgoingSeqNum++));
  logonReply.addField(49, session.senderCompID);
  logonReply.addField(56, session.targetCompID);
  logonReply.addField(98, "0");
  logonReply.addField(108, "30");
  
  sendMessage(logonReply);
}

bool FixSessionManager::isLoggedOn() {
  return session.isLoggedOn;
}

void FixSessionManager::registerHandlers() {
  dispatcher.registerHandler("A", std::make_unique<LogonFixMessageHandler>());
}
