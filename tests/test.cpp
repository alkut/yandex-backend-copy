#include <gtest/gtest.h>
#include "../application/Application.hpp"
#include "../application/EchoServer.hpp"
#include "../Configuration.h"

TEST(Test, Positive) {
    ASSERT_EQ(0, 0);
}


TEST(echo_server, start) {
    Application <EchoServer> app;
}

int main(int argc, char **argv) {
    InitLogging(argv);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
