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

query_namespace::Query MakeQuery(const std::string& Uri, const std::string& body);

template<class Responder>
void event_loop(boost::asio::ip::tcp::acceptor& acceptor,
                boost::asio::ip::tcp::socket& socket);

template<class Responder>
void runServer();

template<class Responder>
class Application2 {
public:
    Application2() = default;
    void run() {
        runServer<Responder>();
    }
};

template<class Responder>
class BoostApplication: public std::enable_shared_from_this<BoostApplication<Responder>> {
public:
    const static inline std::string host = "127.0.0.1";
    const static unsigned int port = 8080;

    explicit BoostApplication(boost::asio::ip::tcp::socket socket): socket_(std::move(socket)) {}
    void start();
    ~BoostApplication();
private:
    Responder* responder = new Responder();
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_{8192};
    boost::beast::http::request<boost::beast::http::string_body> request_;
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;

    void read_request();
    void process_request();
    void create_response();
    void write_response();
};

template<class Responder>
void BoostApplication<Responder>::start() {
    read_request();
}

template<class Responder>
void BoostApplication<Responder>::read_request() {
    auto self = this->shared_from_this();

    boost::beast::http::async_read(
            socket_,
            buffer_,
            request_,
            [self](boost::beast::error_code ec,
                   std::size_t bytes_transferred)
            {
                boost::ignore_unused(bytes_transferred);
                if(!ec)
                    self->process_request();
            });
}

template<class Responder>
void BoostApplication<Responder>::process_request() {
    response_.version(request_.version());
    response_.keep_alive(false);
    response_.set(boost::beast::http::field::server, "Beast");
    create_response();
    write_response();
}

template<class Responder>
void BoostApplication<Responder>::create_response() {
    std::string body = request_.body();
    std::string url(request_.target());
    //auto method = request_.method();
    auto query = MakeQuery(url, body);
    auto respond = reinterpret_cast<QueryResponder*>(responder)->Response(query);
    response_.set(boost::beast::http::field::content_type, "text/html");
    ///TODO change type of respond.code
    response_.result(respond.code);
    boost::beast::ostream(response_.body()) << respond.message;
}

template<class Responder>
void BoostApplication<Responder>::write_response() {
    auto self = this->shared_from_this();
    response_.content_length(response_.body().size());
    boost::beast::http::async_write(
            socket_,
            response_,
            [self](boost::beast::error_code ec, std::size_t)
            {
                self->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
            });
}

template<class Responder>
BoostApplication<Responder>::~BoostApplication() {
    delete responder;
}




template<class Responder>
void event_loop(boost::asio::ip::tcp::acceptor &acceptor, boost::asio::ip::tcp::socket &socket) {
    acceptor.async_accept(socket,
                          [&](boost::beast::error_code ec)
                          {
                              if(!ec)
                                  std::make_shared<BoostApplication<Responder>>(std::move(socket))->start();
                              event_loop<Responder>(acceptor, socket);
                          });
}

template<class Responder>
void runServer() {
    try {
        auto const address = boost::asio::ip::make_address(BoostApplication<Responder>::host);
        boost::asio::io_context ioc(1);
        boost::asio::ip::tcp::acceptor acceptor{ioc, {address, BoostApplication<Responder>::port}};
        boost::asio::ip::tcp::socket socket{ioc};
        event_loop<Responder>(acceptor, socket);
        ioc.run();
    }
    catch(std::exception const& e) {
        LOG(ERROR) << "something happened while starting server...\n" << e.what();
        throw e;
    }
}

#endif //LIB_BOOST_APPLICATION_H
