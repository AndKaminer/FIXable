#include <chrono>
#include <string>

struct FixSessionState {
  std::string senderCompID;
  std::string targetCompID;
  int incomingSeqNum = 1;
  int outgoingSeqNum = 1;
  bool isLoggedOn = false;
  std::chrono::steady_clock::time_point lastReceived;
};
