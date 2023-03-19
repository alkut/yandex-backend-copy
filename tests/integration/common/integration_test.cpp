#include <gtest/gtest.h>
#include <mutex>
#include <thread>

#include "../../../src/application/BoostApplication.hpp"
#include "../../utils/curl/ExecuteCurl.hpp"
#include "../../utils/echo/EchoServer.hpp"

TEST(Test, Positive) {
    ASSERT_EQ(0, 0);
}

TEST(echo_server, url) {
    std::string query_route = "/imports";
    std::string query_url = "http://localhost:8080/imports";
    std::unordered_map<std::string, std::string> query_params = {};
    std::string query_body = "";
    std::string resp = yad_server::tests::ExecuteCurl(query_url, query_params, query_body);
    auto result = nlohmann::json::parse(resp);
    std::string res = result[0].at("url");
    ASSERT_EQ(query_route, res);
}

TEST(echo_server, params) {
    std::string route = "/imports";
    std::string query_url = "http://localhost:8080/imports";
    std::unordered_map<std::string, std::string> query_params = {{"a", "3"},
                                                                 {"b", "4"}};
    std::string query_body = "";
    std::string resp = yad_server::tests::ExecuteCurl(query_url, query_params, query_body);
    auto result = nlohmann::json::parse(resp);
    std::unordered_map<std::string, std::string> res = result[0].at("params");
    ASSERT_EQ(res, query_params);
}

TEST(echo_server, body) {
    std::string route = "/imports";
    std::string query_url = "http://localhost:8080/imports";
    std::unordered_map<std::string, std::string> query_params = {};
    std::string query_body = "z=10&x=15";
    std::string resp = yad_server::tests::ExecuteCurl(query_url, query_params, query_body);
    auto result = nlohmann::json::parse(resp);
    std::string res = result[0].at("body");
    ASSERT_EQ(query_body, res);
}

TEST(echo_server, all) {
    std::string query_route = "/imports";
    std::string query_url = "http://localhost:8080/imports";
    std::unordered_map<std::string, std::string> query_params = {{"mango",    "apple"},
                                                                 {"cucumber", "banana"}};
    std::string query_body = "A=1&B=2";
    std::string resp = yad_server::tests::ExecuteCurl(query_url, query_params, query_body);
    auto result = nlohmann::json::parse(resp);
    std::string res_route = result[0].at("url");
    std::unordered_map<std::string, std::string> res_params = result[0].at("params");
    std::string res_body = result[0].at("body");
    ASSERT_EQ(query_route, res_route);
    ASSERT_EQ(query_params, res_params);
    ASSERT_EQ(query_body, res_body);
}

void runserver() {
    yad_server::application::Application<yad_server::tests::EchoServer> app;
    app.run();
}

void runtests(int args, char **argv, int &code) {
    yad_server::logging::InitLogging();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    testing::InitGoogleTest(&args, argv);
    code = RUN_ALL_TESTS();
    yad_server::tests::ExecuteCurl("http://localhost:8080/shutdown", {}, "");
}

int main(int argc, char **argv) {
    yad_server::global::ProgramOptions::setInstance(argc, argv);
    int code;
    std::thread first([]() { runserver(); });
    runtests(argc, argv, code);
    first.detach();
    return code;
}