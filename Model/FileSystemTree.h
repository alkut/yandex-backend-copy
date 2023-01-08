#ifndef SERVER_FILE_SYSTEM_TREE_H
#define SERVER_FILE_SYSTEM_TREE_H

#include "Includes.h"
#include "View/GetNodesBody.h"
#include "Model/HistoryStorage.h"

struct FileSystemTree {
    struct Node {
        Node *parent = nullptr;
        ImportBodyMessage::ImportBodyItem item;
        unordered_map<string , Node*> childrenFiles, childrenFolders;
    };

private:
    HistoryStorage history;
public:
    FileSystemTree() = default;
    unordered_map<string, Node*> position;
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
    static void DecreaseSize(Node *node, int64_t size);
    void UpdateDate(Node *node, const string& date, long long ms);
    void clear(Node *node);
    static vector<Node*> getParents(Node *root);
    static vector<Node*> getChildren(Node *root);
    static void UnlinkParent(Node *);
    enum class Colour {GRAY, BLACK};
};

#endif //SERVER_FILE_SYSTEM_TREE_H
