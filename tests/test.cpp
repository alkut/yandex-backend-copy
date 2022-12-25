#include <gtest/gtest.h>

TEST(Test, Positive) {
    ASSERT_EQ(0, 0);
}

TEST(Test, Negative) {
    ASSERT_EQ(1.0, 0.0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
