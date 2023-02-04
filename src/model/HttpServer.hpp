#ifndef SERVER_HTTPSERVER_H
#define SERVER_HTTPSERVER_H

#include <boost/bind.hpp>

#include "../view/ImportBody.hpp"
#include "../repository/FileSystemRepository.hpp"
#include "../validation/Validator.hpp"
#include "../application/QueryResponder.hpp"
#include "TreeBuilder.hpp"

namespace yad_server::model {
    class HttpServer: application::QueryResponder {
    public:
        application::Respond Response(const application::query::Query &query) override;
        HttpServer() = default;
        HttpServer(const HttpServer &) = delete;
        HttpServer &operator=(const HttpServer &) = delete;
        ~HttpServer() = default;

    private:
        yad_server::repository::FileSystemRepository repository;

        const std::unordered_map<std::string,
        std::function<application::Respond(const validation::QueryExt&)>>
        actions = {{"/import", boost::bind(&HttpServer::HandleImport, this, _1)},
                   {"/delete", boost::bind(&HttpServer::HandleDelete, this, _1)},
                   {"/nodes", boost::bind(&HttpServer::HandleGetNodes, this, _1)}};

        application::Respond HandleImport(const validation::QueryExt&);
        application::Respond HandleDelete(const validation::QueryExt&);
        application::Respond HandleGetNodes(const validation::QueryExt&);

        const application::Respond OK = {HTTP_OK, ""};
        const application::Respond NotFound = {HTTP_NOTFOUND, "item not found"};
        const application::Respond BadRequest = {HTTP_BADREQUEST, "validation failed"};
    };
}
#endif //SERVER_HTTPSERVER_H
