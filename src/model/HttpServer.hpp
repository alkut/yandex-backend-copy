#ifndef SERVER_HTTPSERVER_H
#define SERVER_HTTPSERVER_H

#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "src/view/ImportBody.hpp"
#include "src/model/FileSystemTree.hpp"
#include "src/validation/Validator.hpp"
#include "src/application/QueryResponder.hpp"

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

        void ValidateImport(view::ImportBodyMessage &msg);

        FileSystemTree::Node *ValidateDelete(const std::string &id) const;

        FileSystemTree::Node *ValidateGetNodes(const std::string &id) const;

        static long long ValidateUpdate(const std::string &date);

        void ValidateImportItem(view::ImportBodyMessage::ImportBodyItem &item,
                                const std::unordered_map<std::string, view::ImportBodyMessage::ImportBodyItem> &ids) const;

        void ValidateExistingItem(view::ImportBodyMessage::ImportBodyItem &item) const;

        static void TopologySort(std::vector<view::ImportBodyMessage::ImportBodyItem> &items);

        const application::Respond OK = {HTTP_OK, ""};
        const application::Respond NotFound = {HTTP_NOTFOUND, "item not found"};
        const application::Respond BadRequest = {HTTP_BADREQUEST, "validation failed"};
    };
}
#endif //SERVER_HTTPSERVER_H
