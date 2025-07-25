#include "fix/handlers/TestRequestFixMessageHandler.h"

void TestRequestFixMessageHandler::handle(const FixMessage& msg, FixSessionManager* session) {
  auto testReqID = msg.get(112).value_or("");

  FixMessage heartbeat;
  heartbeat.addField(35, "0");
  heartbeat.addField(34, std::to_string(session->getNextOutgoingSeqNum()));
  heartbeat.addField(49, session->getSenderCompID());
  heartbeat.addField(56, session->getTargetCompID());
  heartbeat.addField(112, testReqID);
  session->sendMessage(heartbeat);
}
