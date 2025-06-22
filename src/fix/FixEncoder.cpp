#include "fix/FixEncoder.h"

std::string FixEncoder::encode(const FixMessage& msg,
                               const std::string& beginString,
                               const char delimiter) {
  FixMessage working = msg;

  working.removeField(8);
  working.removeField(9);
  working.removeField(10);

  std::string body = working.toString();
  int bodyLength = static_cast<int>(body.size());

  std::ostringstream ss;
  ss << "8=" << beginString << delimiter;
  ss << "9=" << bodyLength << delimiter;
  ss << body;

  std::string fullWithoutChecksum = ss.str();

  int checksum = 0;
  for (char c : fullWithoutChecksum) {
    checksum += static_cast<unsigned char>(c);
  }
  checksum %= 256;
  std::ostringstream checksumField;
  checksumField << "10=" << std::setw(3) << std::setfill('0');
  checksumField << checksum << delimiter;

  return fullWithoutChecksum + checksumField.str();
}
