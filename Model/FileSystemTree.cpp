#include "FileSystemTree.h"

void FileSystemTree::Import(ImportBodyMessage& msg) {
    vector<ImportBodyMessage::ImportBodyItem>& items = msg.Items;
    TopologySort(items);
    for (auto& el : items)
        AddItem(el);
}

GetNodesBodyMessage FileSystemTree::GetNodes(Node *node) const
{
    GetNodesBodyMessage Response;
    Response.body = node->item;
    vector<GetNodesBodyMessage> children;
    for (const auto& [id, ptr] : node->childrenFolders)
        children.push_back(GetNodes(ptr));
    for (const auto& [id, ptr] : node->childrenFiles)
        children.push_back(GetNodesBodyMessage{position.find(id)->second->item, vector<GetNodesBodyMessage>()});
    Response.children = children;
    return Response;
}

void FileSystemTree::Delete(Node *node, const string& date, long long ms) {
    DecreaseSize(node->parent, node->item.size);
    UpdateDate(node->parent, date, ms);

    if (node->parent != nullptr) {
        if (node->item._systemItemType == SystemItemType::FILE)
            node->parent->childrenFiles.erase(node->item.id);
        else
            node->parent->childrenFolders.erase(node->item.id);
    }

    clear(node);
}

vector<ImportBodyMessage::ImportBodyItem> FileSystemTree::Update(long long ms) {
    history.DeleteOld(ms);
    return history.GetAll();
}

void FileSystemTree::TopologySort(vector<ImportBodyMessage::ImportBodyItem>& items)
{
    Graph G;
    auto CopyItems = items;
    std::unordered_map<string, int> IdToIndex;
    for (int i = 0; i < items.size(); ++i)
    {
        IdToIndex[items[i].id] = i;
    }

    unordered_set<int> Outside;
    for (int i = 0; i < items.size(); ++i)
        Outside.insert(i);

    for (int i = 0; i < items.size(); ++i)
    {
        auto it = IdToIndex.find(items[i].parentId);
        if (it != IdToIndex.end()) {
            add_edge(i, it->second, G);
            Outside.erase(i);
            Outside.erase(it->second);
        }
    }

    container c;
    topological_sort(G, std::back_inserter(c));

    vector<int> ParentInsideQuery;

    for (const auto& ii: c)
        ParentInsideQuery.push_back(static_cast<int>(ii));

    if (Outside.empty()) {
        for (int i = 0; i < items.size(); ++i)
            items[i] = CopyItems[ParentInsideQuery[i]];
        return;
    }

    vector<int> ParentOutsideQuery(Outside.begin(), Outside.end());

    for (int i = 0; i < ParentOutsideQuery.size(); ++i)
        items[i] = CopyItems[ParentOutsideQuery[i]];
    for (size_t i = ParentOutsideQuery.size(); i < items.size(); ++i)
        items[i] = CopyItems[ParentInsideQuery[i]];
}

void FileSystemTree::AddItem(ImportBodyMessage::ImportBodyItem& item) {
    if (position.find(item.id) != position.end()) {
        Override(item);
        LOG(INFO) << "Item added with id: " << item.id;
        return;
    }
    Node *NewNode = new Node;
    if (item._systemItemType == SystemItemType::FILE) {
        history.Add(item);
    }
    position[item.id] = NewNode;

    if (item.parentId.empty())
    {
        NewNode->parent = nullptr;
        LOG(INFO) << "Item added with id: " << item.id;
        return;
    }

    auto parentNode = position[item.parentId];
    NewNode->parent = parentNode;
    UpdateDate(parentNode, item.date, item.date_ms);

    DecreaseSize(parentNode, -item.size);

    switch (item._systemItemType) {
        case SystemItemType::FOLDER:
            parentNode->childrenFolders[item.id] = NewNode;
            break;
        case SystemItemType::FILE:
            parentNode->childrenFiles[item.id] = NewNode;
            break;
    }
    LOG(INFO) << "Item added with id: " << item.id;
}

void FileSystemTree::Override(const ImportBodyMessage::ImportBodyItem& item) {
    Node *Node = position[item.id];
    if (item._systemItemType == SystemItemType::FILE) {
        history.Add(item);
    }
    auto OldParentNode = Node->parent;
    if (OldParentNode != nullptr)
        DecreaseSize(OldParentNode, item.size);
    auto NewParentNode = position[item.parentId];
    if (NewParentNode != nullptr)
        DecreaseSize(NewParentNode, -item.size);

    UpdateDate(OldParentNode, item.date, item.date_ms);
    UpdateDate(NewParentNode, item.date, item.date_ms);

    if (OldParentNode == NewParentNode) {
        return;
    }
    switch(item._systemItemType)
    {
        case SystemItemType::FOLDER:
            if (OldParentNode != nullptr)
                OldParentNode->childrenFolders.erase(item.id);
            if (NewParentNode != nullptr)
                NewParentNode->childrenFolders[item.id] = Node;
            break;
        case SystemItemType::FILE:
            if (OldParentNode != nullptr)
                OldParentNode->childrenFiles.erase(item.id);
            if (NewParentNode != nullptr)
                NewParentNode->childrenFolders[item.id] = Node;
            break;
    }
}

void FileSystemTree::DecreaseSize(Node *node, int64_t size)
{
    while (node != nullptr)
    {
        node->item.size -= size;
        node = node->parent;
    }
}

void FileSystemTree::UpdateDate(Node *node, const string& date, long long ms)
{
    while (node != nullptr)
    {
        auto& it = node->item;
        it.date = date;
        it.date_ms = ms;
        node = node->parent;
        if (it._systemItemType == SystemItemType::FILE)
        {
            history.Add(it);
        }
    }
}

vector<ImportBodyMessage::ImportBodyItem> FileSystemTree::GetNodeHistory(Node *node, long long start, long long end) const {
    vector<ImportBodyMessage::ImportBodyItem> Response;
    //TODO
    return Response;
}

void FileSystemTree::clear(Node *node) {
    for (auto& [id, ptr]: node->childrenFiles)
        if (ptr != nullptr)
            clear(ptr);
    for (auto& [id, ptr]: node->childrenFolders)
        if (ptr != nullptr)
            clear(ptr);
    if (node->item._systemItemType == SystemItemType::FILE){
        history.Remove(node->item.id);
    }
    position.erase(node->item.id);
    delete node;
}

FileSystemTree::~FileSystemTree() {
    vector<Node*> roots;
    for (const auto& it : position)
        if (it.second->parent == nullptr)
            roots.push_back(it.second);
    for (auto& it: roots)
        clear(it);
}