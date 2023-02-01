#include <gtest/gtest.h>

TEST(Test, Positive) {
    ASSERT_EQ(0, 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
