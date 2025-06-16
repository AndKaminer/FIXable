#include <gtest/gtest.h>
#include "fix/FixMessage.h"

TEST(FixMessageTest, FieldAddGet) {
    FixMessage msg;
    msg.addField(35, "D");
    msg.addField(49, "CLIENT");

    EXPECT_TRUE(msg.has(35));
    EXPECT_EQ(msg.get(35).value(), "D");
    EXPECT_EQ(msg.get(49).value(), "CLIENT");
    EXPECT_FALSE(msg.has(56));
}
