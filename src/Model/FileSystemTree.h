#ifndef SERVER_FILE_SYSTEM_TREE_H
#define SERVER_FILE_SYSTEM_TREE_H

#include <utility>

#include "src/Includes.h"
#include "src/View/GetNodesBody.h"
#include "HistoryStorage.h"

struct FileSystemTree {
    struct Node {
        Node *parent = nullptr;
        ImportBodyMessage::ImportBodyItem item;
        unordered_map<string , Node*> childrenFiles, childrenFolders;
        explicit Node(const ImportBodyMessage::ImportBodyItem& item);
        string& id();
        string& date();
        long long& date_ms();
        int64_t& size();
        const SystemItemType& type() const;
    };

private:
    HistoryStorage history;
    unordered_map<string, Node*> position;
public:
    friend class HttpServer;
    FileSystemTree() = default;
    void Import(ImportBodyMessage& msg);
    GetNodesBodyMessage GetNodes(Node *node) const;
    void Delete(Node *node, const string& date, long long ms);
    vector<ImportBodyMessage::ImportBodyItem> Update(long long ms);
    static vector<ImportBodyMessage::ImportBodyItem> GetNodeHistory(Node *node, long long start, long long end) ;
    ~FileSystemTree();
private:
    static void TopologySort(vector<ImportBodyMessage::ImportBodyItem>& items);
    void AddItem(ImportBodyMessage::ImportBodyItem& item);
    void Override(const ImportBodyMessage::ImportBodyItem& item);
    static vector<Node*> getParents(Node *root);
    static vector<Node*> getChildren(Node *root);
    void DestroyNode(Node *node);
    void UnlinkNode(Node *root, const string& date, long long ms);
    void UnlinkNode(Node *root);
    static void LinkNode(Node *root, Node *parent, const string& date, long long ms);
    static void LinkNode(Node *root, Node *parent);
};

#endif //SERVER_FILE_SYSTEM_TREE_H
