#include <gtest/gtest.h>
#include "../application/Application.hpp"

TEST(Test, Positive) {
    ASSERT_EQ(0, 0);
}


TEST(echo_server, start) {

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
