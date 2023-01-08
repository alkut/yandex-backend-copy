#include <gtest/gtest.h>
#include "../application/Application.hpp"
<<<<<<< HEAD
<<<<<<< HEAD
#include "EchoServer.hpp"
#include <curl/curl.h>
#include "../Configuration.h"
=======
>>>>>>> fc06ffd (YAS-12-add-application-template-class)
=======
#include "../application/EchoServer.hpp"
#include <curl/curl.h>
#include "../Configuration.h"
>>>>>>> ba6314f (YAS-12-add-application-template-class)

TEST(Test, Positive) {
    ASSERT_EQ(0, 0);
}

<<<<<<< HEAD

TEST(echo_server, start) {
    Application <EchoServer> app;
}
=======
>>>>>>> fc06ffd (YAS-12-add-application-template-class)

TEST(echo_server, start) {
    Application <EchoServer> app;
    // app.run();
}

int main(int argc, char **argv) {
    InitLogging(argv);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
