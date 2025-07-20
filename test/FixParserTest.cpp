#include "fix/FixParser.h"
#include "fix/FixEncoder.h"
#include "fix/FixMessage.h"

#include <gtest/gtest.h>
#include <string>

TEST(FixParserTest, ParsesValidMessage) {
    FixMessage msg;
    msg.addField(35, "D");
    msg.addField(49, "SENDER");
    msg.addField(56, "TARGET");

    std::string encoded = FixEncoder::encode(msg, "FIX.4.2");

    FixParser parser('\x01');
    auto result = parser.parseMessage(encoded);

    ASSERT_TRUE(result.has_value());
    FixMessage parsed = result.value();

    EXPECT_EQ(parsed.get(8).value_or(""), "FIX.4.2");
    EXPECT_EQ(parsed.get(35).value_or(""), "D");
    EXPECT_EQ(parsed.get(49).value_or(""), "SENDER");
    EXPECT_EQ(parsed.get(56).value_or(""), "TARGET");
    EXPECT_TRUE(parsed.isValid(true));
}

TEST(FixParserTest, FailsOnMissingEquals) {
    std::string badInput = "8FIX.4.2\x01";  // no '='
    FixParser parser('\x01');
    auto result = parser.parseMessage(badInput);
    EXPECT_FALSE(result.has_value());
}

TEST(FixParserTest, FailsOnNonNumericTag) {
    std::string badInput = "ABC=123\x01";
    FixParser parser('\x01');
    auto result = parser.parseMessage(badInput);
    EXPECT_FALSE(result.has_value());
}

TEST(FixParserTest, FailsOnInvalidMessageFormat) {
    // 8 and 9 are missing → fails validity check
    std::string input = "35=A\x01";
    FixParser parser('\x01');
    auto result = parser.parseMessage(input);
    EXPECT_FALSE(result.has_value());
}

TEST(FixParserTest, EmptyInputReturnsInvalid) {
    FixParser parser('\x01');
    auto result = parser.parseMessage("");
    EXPECT_FALSE(result.has_value());
}

TEST(FixParserTest, PartialTagValueIsHandledCorrectly) {
    std::string input = "8=FIX.4.2" "\x01" "9=" "\x01" "35=D" "\x01" "10=000" "\x01";
    FixParser parser('\x01');
    auto result = parser.parseMessage(input);

    ASSERT_FALSE(result.has_value());
}

TEST(FixParserTest, HandlesTrailingDelimiter) {
    std::string input = "8=FIX.4.2" "\x01" "9=5" "\x01" "35=0" "\x01" "10=123" "\x01";
    FixParser parser('\x01');
    auto result = parser.parseMessage(input);
    EXPECT_TRUE(result.has_value());
}

