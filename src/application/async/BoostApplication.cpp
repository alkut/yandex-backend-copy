#include "src/application/async/BoostApplication.hpp"

void BoostApplication::start() {
    read_request();
}

void BoostApplication::read_request() {
    auto self = shared_from_this();
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

void BoostApplication::process_request() {
    response_.version(request_.version());
    response_.keep_alive(false);
    response_.set(boost::beast::http::field::server, "Beast");
    create_response();
    write_response();
}

void BoostApplication::create_response() {
    std::string body = request_.body();
    std::string url(request_.target());
    //auto method = request_.method();
    auto query = MakeQuery(url, body);
    auto respond = server.Response(query);
    response_.set(boost::beast::http::field::content_type, "text/html");
    ///TODO change type of respond.code
    response_.result(respond.code);
    boost::beast::ostream(response_.body()) << respond.message;
}

void BoostApplication::write_response() {
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

void event_loop(boost::asio::ip::tcp::acceptor &acceptor, boost::asio::ip::tcp::socket &socket) {
    acceptor.async_accept(socket,
                          [&](boost::beast::error_code ec)
                          {
                              if(!ec)
                                  std::make_shared<BoostApplication>(std::move(socket))->start();
                              event_loop(acceptor, socket);
                          });
}

void runServer() {
    try {
        auto const address = boost::asio::ip::make_address(BoostApplication::host);
        boost::asio::io_context ioc(1);
        boost::asio::ip::tcp::acceptor acceptor{ioc, {address, BoostApplication::port}};
        boost::asio::ip::tcp::socket socket{ioc};
        event_loop(acceptor, socket);
        ioc.run();
    }
    catch(std::exception const& e) {
        LOG(ERROR) << "something happened while starting server...\n" << e.what();
        throw e;
    }
}

query::Query MakeQuery(const std::string& Uri, const std::string& body) {
    std::vector<std::string> splittedUri;
    boost::split(splittedUri, Uri, boost::is_any_of("?"));
    if (splittedUri.size() == 1) {
        return {Uri, {}, body};
    }
    if (splittedUri.size() > 2) {
        LOG(ERROR) << "invalid uri: ";
        throw std::invalid_argument( "invalid uri.");
    }
    std::unordered_map<std::string, std::string> params_um;
    std::vector<std::string> params_vector;
    boost::split(params_vector, splittedUri[1], boost::is_any_of("&"));
    for (const auto &params: params_vector) {
        std::vector<std::string> key_value;
        boost::split(key_value, params, boost::is_any_of("="));
        if (key_value.size() != 2)
        {
            LOG(ERROR) << "invalid pair of parameters in query: " << params;
            throw std::invalid_argument( "invalid pair of parameters in query.");

        }
        params_um[key_value[0]] = key_value[1];
    }
    return {splittedUri[0], params_um, body};
}
