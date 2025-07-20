#include <gtest/gtest.h>

#include <regex>
#include <string>

#include "fix/FixEncoder.h"
#include "fix/FixMessage.h"

// Helper to extract tag value using regex
std::string getTagValue(const std::string& fixStr, int tag) {
  std::regex tagRegex(std::to_string(tag) + R"(=(.*?)\x01)");
  std::smatch match;
  if (std::regex_search(fixStr, match, tagRegex)) {
    return match[1].str();
  }
  return "";
}

TEST(FixEncoderTest, EncodesBasicMessageCorrectly) {
  FixMessage msg;
  msg.addField(35, "D");
  msg.addField(49, "SENDER");
  msg.addField(56, "TARGET");

  std::string encoded = FixEncoder::encode(msg, "FIX.4.2");

  // Header checks
  EXPECT_TRUE(encoded.rfind("8=FIX.4.2\x01", 0) ==
              0);  // Starts with BeginString

  // Required fields exist
  EXPECT_NE(encoded.find("9="), std::string::npos);
  EXPECT_NE(encoded.find("10="), std::string::npos);

  // Fields present
  EXPECT_NE(encoded.find("35=D\x01"), std::string::npos);
  EXPECT_NE(encoded.find("49=SENDER\x01"), std::string::npos);
  EXPECT_NE(encoded.find("56=TARGET\x01"), std::string::npos);

  // Validate FIX message format and checksum
  FixMessage parsed;
  parsed.addField(8, getTagValue(encoded, 8));
  parsed.addField(9, getTagValue(encoded, 9));
  parsed.addField(35, getTagValue(encoded, 35));
  parsed.addField(49, getTagValue(encoded, 49));
  parsed.addField(56, getTagValue(encoded, 56));
  parsed.addField(10, getTagValue(encoded, 10));
  EXPECT_TRUE(parsed.isValid(true));
}

TEST(FixEncoderTest, OverwritesFields8_9_10) {
  FixMessage msg;
  msg.addField(8, "BAD");
  msg.addField(9, "1234");
  msg.addField(10, "999");
  msg.addField(35, "A");

  std::string encoded = FixEncoder::encode(msg, "FIX.4.4");

  // Should not contain the original 8/9/10 values
  EXPECT_EQ(getTagValue(encoded, 8), "FIX.4.4");
  EXPECT_NE(getTagValue(encoded, 9), "1234");
  EXPECT_NE(getTagValue(encoded, 10), "999");

  // Still valid
  FixMessage parsed;
  parsed.addField(8, getTagValue(encoded, 8));
  parsed.addField(9, getTagValue(encoded, 9));
  parsed.addField(35, getTagValue(encoded, 35));
  parsed.addField(10, getTagValue(encoded, 10));
  EXPECT_TRUE(parsed.isValid(true));
}

TEST(FixEncoderTest, ProducesCorrectChecksum) {
  FixMessage msg;
  msg.addField(35, "0");

  std::string encoded = FixEncoder::encode(msg, "FIX.4.2");
  std::string checksumStr = getTagValue(encoded, 10);

  // Recompute expected checksum
  size_t end = encoded.rfind("10=");
  std::string withoutChecksum = encoded.substr(0, end);

  int sum = 0;
  for (char c : withoutChecksum) {
    sum += static_cast<unsigned char>(c);
  }
  int expectedChecksum = sum % 256;

  std::ostringstream oss;
  oss << std::setw(3) << std::setfill('0') << expectedChecksum;

  EXPECT_EQ(checksumStr, oss.str());
}

TEST(FixEncoderTest, EmptyMessageYieldsValidOutput) {
  FixMessage msg;  // No fields

  std::string encoded = FixEncoder::encode(msg, "FIX.4.2");

  std::string bodyLength = getTagValue(encoded, 9);
  std::string checksum = getTagValue(encoded, 10);

  EXPECT_EQ(getTagValue(encoded, 8), "FIX.4.2");
  EXPECT_FALSE(bodyLength.empty());
  EXPECT_FALSE(checksum.empty());
}

TEST(FixEncoderTest, HandlesMultipleFieldsAndOrdering) {
  FixMessage msg;
  msg.addField(35, "8");  // Execution report
  msg.addField(49, "BUY_SIDE");
  msg.addField(56, "SELL_SIDE");
  msg.addField(34, "2");
  msg.addField(52, "20250720-14:00:00");

  std::string encoded = FixEncoder::encode(msg, "FIX.4.4");

  EXPECT_NE(encoded.find("35=8\x01"), std::string::npos);
  EXPECT_NE(encoded.find("49=BUY_SIDE\x01"), std::string::npos);
  EXPECT_NE(encoded.find("56=SELL_SIDE\x01"), std::string::npos);
  EXPECT_NE(encoded.find("34=2\x01"), std::string::npos);
  EXPECT_NE(encoded.find("52=20250720-14:00:00\x01"), std::string::npos);

  // Valid FIX structure
  FixMessage parsed;
  parsed.addField(8, getTagValue(encoded, 8));
  parsed.addField(9, getTagValue(encoded, 9));
  parsed.addField(35, getTagValue(encoded, 35));
  parsed.addField(34, getTagValue(encoded, 34));
  parsed.addField(49, getTagValue(encoded, 49));
  parsed.addField(52, getTagValue(encoded, 52));
  parsed.addField(56, getTagValue(encoded, 56));
  parsed.addField(10, getTagValue(encoded, 10));
  EXPECT_TRUE(parsed.isValid(true));
}
