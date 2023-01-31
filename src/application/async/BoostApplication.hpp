#ifndef LIB_BOOST_APPLICATION_H
#define LIB_BOOST_APPLICATION_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "src/logging/init.hpp"
#include "src/application/Query.hpp"
#include "src/application/QueryResponder.hpp"
#include "src/model/HttpServer.hpp"

query::Query MakeQuery(const std::string& Uri, const std::string& body);

void event_loop(boost::asio::ip::tcp::acceptor& acceptor,
                boost::asio::ip::tcp::socket& socket);

void runServer();

class Application2 {
public:
    Application2() = default;
    static void run() {
        runServer();
    }
};

class BoostApplication: public std::enable_shared_from_this<BoostApplication> {
public:
    explicit BoostApplication(boost::asio::ip::tcp::socket socket): socket_(std::move(socket)) {}
    void start();
    static const inline std::string host = "127.0.0.1";
    static const unsigned int port = 8080;
private:
    HttpServer server;
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_{8192};
    boost::beast::http::request<boost::beast::http::string_body> request_;
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;

    void read_request();
    void process_request();
    void create_response();
    void write_response();
};

#endif //LIB_BOOST_APPLICATION_H
