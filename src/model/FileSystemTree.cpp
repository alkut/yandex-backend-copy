#include "FileSystemTree.hpp"

void FileSystemTree::Import(ImportBodyMessage& msg) {
    for (const auto& item : msg.Items)
        AddItem(item);
}

GetNodesBodyMessage FileSystemTree::GetNodes(Node *node) const
{
    GetNodesBodyMessage Response;
    Response.body = node->item;
    vector<GetNodesBodyMessage> children;
    for (const auto& [id, ptr] : node->childrenFolders)
        children.push_back(GetNodes(ptr));
    for (const auto& [id, ptr] : node->childrenFiles)
        children.push_back(GetNodesBodyMessage{ptr->item, vector<GetNodesBodyMessage>()});
    Response.children = children;
    return Response;
}

void FileSystemTree::Delete(Node *node, const string& date, long long ms) {
    UnlinkNode(node, date, ms);
    DestroyNode(node);
}

vector<ImportBodyMessage::ImportBodyItem> FileSystemTree::Update(long long ms) {
    history.DeleteOld(ms);
    return history.GetAll();
}

void FileSystemTree::AddItem(const ImportBodyMessage::ImportBodyItem& item) {
    if (position.find(item.id) != position.end()) {
        Override(item);
        LOG(INFO) << "Item added with id: " << item.id;
        return;
    }
    history.Add(item);
    Node *NewNode = new Node(item);
    position[item.id] = NewNode;
    auto parentNode = item.parentId.empty() ? nullptr:
            position[item.parentId];
    LinkNode(NewNode, parentNode);
    LOG(INFO) << "Item added with id: " << item.id;
}

void FileSystemTree::Override(const ImportBodyMessage::ImportBodyItem& item) {
    history.Add(item);
    Node *Node = position[item.id];
    auto it = position.find(item.id);
    auto parentNode = (it != position.end()) ? it->second : nullptr;
    UnlinkNode(Node);
    LinkNode(Node, parentNode);
}

FileSystemTree::~FileSystemTree() {
    vector<Node*> roots;
    for (const auto& [id, ptr] : position)
        if (ptr->parent == nullptr)
            roots.push_back(ptr);
    for (auto& it: roots)
        DestroyNode(it);
}

///get all parents exclude root itself
vector<FileSystemTree::Node *> FileSystemTree::getParents(FileSystemTree::Node *root) {
    vector<FileSystemTree::Node *> ans;
    root = root->parent;
    while (root) {
        ans.push_back(root);
        root = root->parent;
    }
    return ans;
}

///get all children include root itself
vector<FileSystemTree::Node *> FileSystemTree::getChildren(FileSystemTree::Node *root) {
    vector<FileSystemTree::Node *> ans;
    std::queue<FileSystemTree::Node *> queue;
    queue.push(root);
    ans.push_back(root);
    while (!queue.empty()) {
        auto top = queue.back();
        for (const auto& [name, ptr]: top->childrenFiles) {
            ans.push_back(ptr);
        }
        for (const auto& [name, ptr]: top->childrenFolders) {
            ans.push_back(ptr);
            queue.push(ptr);
        }
        queue.pop();
    }
    return ans;
}

void FileSystemTree::DestroyNode(Node *node) {
    auto children = getChildren(node);
    for (auto& ptr: children) {
        history.Remove(ptr->item);
        position.erase(ptr->id());
        delete ptr;
    }
}

void FileSystemTree::UnlinkNode(FileSystemTree::Node *root, const string& date, long long ms) {
    if (root == nullptr)
        return;
    auto parents = getParents(root);
    for (auto& ptr: parents) {
        ptr->size() -= root->size();
        ptr->date() = date;
        ptr->date_ms() = ms;
        history.Add(ptr->item);
    }
    if (root->type() == SystemItemType::FILE)
        root->parent->childrenFiles.erase(root->id());
    else
        root->parent->childrenFolders.erase(root->id());
}

void FileSystemTree::UnlinkNode(FileSystemTree::Node *root) {
    if (root == nullptr)
        return;
    UnlinkNode(root, root->date(), root->date_ms());
}

void FileSystemTree::LinkNode(FileSystemTree::Node *root, FileSystemTree::Node *parent, const string &date,
                              long long ms) {
    if (root == nullptr || parent == nullptr)
        return;
    root->parent = parent;
    if (root->type() == SystemItemType::FILE)
        parent->childrenFiles[root->id()] = root;
    else
        parent->childrenFolders[root->id()] = root;

    auto parents = getParents(root);
    for (auto& ptr: parents) {
        ptr->date() = date;
        ptr->date_ms() = ms;
        ptr->size() += root->size();
    }
}

void FileSystemTree::LinkNode(FileSystemTree::Node *root, FileSystemTree::Node *parent) {
    if (root == nullptr || parent == nullptr)
        return;
    LinkNode(root, parent, root->date(), root->date_ms());
}

bool FileSystemTree::IsParent(const string &node_id, const string &parent_id) const {
    if (position.find(node_id) == position.end() || position.find(parent_id) == position.end())
        return false;
    auto node = position.find(node_id)->second;
    auto parent = position.find(parent_id)->second;
    while (node != nullptr) {
        if (node == parent)
            return true;
        node = node->parent;
    }
    return false;
}

FileSystemTree::Node::Node(const ImportBodyMessage::ImportBodyItem &item) : item(item) {}

string &FileSystemTree::Node::id() {
    return item.id;
}

string &FileSystemTree::Node::date() {
    return item.date;
}

long long &FileSystemTree::Node::date_ms() {
    return item.date_ms;
}

int64_t &FileSystemTree::Node::size() {
    return item.size;
}

const SystemItemType &FileSystemTree::Node::type() const {
    return item._systemItemType;
}