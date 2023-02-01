#ifndef SERVER_HTTPSERVER_H
#define SERVER_HTTPSERVER_H

#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../view/ImportBody.hpp"
#include "../model/FileSystemTree.hpp"
#include "../validation/Validator.hpp"
#include "../application/QueryResponder.hpp"

namespace yad_server::model {
    class NotFoundException : std::exception {
    public:
        explicit NotFoundException(const std::string &msg);
    };
    class HttpServer : application::QueryResponder {
    public:
        application::Respond Response(const application::query::Query &query) override;

        HttpServer() = default;

        HttpServer(const HttpServer &) = delete;

        HttpServer &operator=(const HttpServer &) = delete;

        ~HttpServer() = default;

    private:
        FileSystemTree file_system;
        validation::Validator validator;
        long long max_date = LLONG_MIN;

        void ValidateImport(view::import_body_message::ImportBodyMessage &msg);

        FileSystemTree::Node *ValidateDelete(const std::string &id) const;

        FileSystemTree::Node *ValidateGetNodes(const std::string &id) const;

        static long long ValidateUpdate(const std::string &date);

        void ValidateImportItem(view::import_body_message::ImportBodyMessage::ImportBodyItem &item,
                                const std::unordered_map<std::string, view::import_body_message::ImportBodyMessage::ImportBodyItem> &ids) const;

        void ValidateExistingItem(view::import_body_message::ImportBodyMessage::ImportBodyItem &item) const;

        static void TopologySort(std::vector<view::import_body_message::ImportBodyMessage::ImportBodyItem> &items);

        const application::Respond OK = {HTTP_OK, ""};
        const application::Respond NotFound = {HTTP_NOTFOUND, "item not found"};
        const application::Respond BadRequest = {HTTP_BADREQUEST, "validation failed"};
    };
}
#endif //SERVER_HTTPSERVER_H
