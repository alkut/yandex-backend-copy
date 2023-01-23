#ifndef SERVER_HTTPSERVER_H
#define SERVER_HTTPSERVER_H

#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "src/view/ImportBody.hpp"
#include "src/model/FileSystemTree.hpp"
#include "src/validation/Validator.hpp"
#include "src/application/QueryResponder.hpp"

typedef boost::adjacency_list<> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::vector<Vertex> container;

class NotFoundException: std::exception {
public:
    explicit NotFoundException(const string& msg);
};

class HttpServer: QueryResponder {
public:
    Respond Response(const query::Query& query) override;
    HttpServer() = default;
    HttpServer(const HttpServer&) = delete;
    HttpServer& operator = (const HttpServer&) = delete;
    ~HttpServer() = default;
private:
    FileSystemTree file_system;
    Validator validator;
    long long max_date = LLONG_MIN;
    void ValidateImport(ImportBodyMessage &msg);
    FileSystemTree::Node *ValidateDelete(const string& id) const;
    FileSystemTree::Node *ValidateGetNodes(const string& id) const;
    static long long ValidateUpdate(const std::string& date);
    void ValidateImportItem(ImportBodyMessage::ImportBodyItem &item, const std::unordered_map<std::string, ImportBodyMessage::ImportBodyItem> &ids) const;
    void ValidateExistingItem(ImportBodyMessage::ImportBodyItem &item) const;
    static void TopologySort(vector<ImportBodyMessage::ImportBodyItem>& items);

    const Respond OK = {HTTP_OK, ""};
    const Respond NotFound = {HTTP_NOTFOUND, "item not found"};
    const Respond BadRequest = {HTTP_BADREQUEST, "validation failed"};
};

#endif //SERVER_HTTPSERVER_H
