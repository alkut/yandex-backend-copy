#ifndef SERVER_FILESYSTEMTREE_H
#define SERVER_FILESYSTEMTREE_H

#include "Includes.h"
#include "View/GetNodesBody.h"
#include "Model/HistoryStorage.h"

struct FileSystemTree
{
    struct Node
    {
        Node *parent = nullptr;
        unsigned long long ShortId;
        unordered_map<unsigned long long, Node*> childrenFiles, childrenFolders;
    };

    void Add(ImportBodyMessage& msg)
    {
        vector<ImportBodyMessage::ImportBodyItem>& items = msg.Items;
        TopologySort(items);
        for (auto& el : items)
            AddItem(el);
    }

    static void TopologySort(vector<ImportBodyMessage::ImportBodyItem>& items)
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
           ParentInsideQuery.push_back(ii);

        if (Outside.empty()) {
            for (int i = 0; i < items.size(); ++i)
                items[i] = CopyItems[ParentInsideQuery[i]];
            return;
        }

        vector<int> ParentOutsideQuery(Outside.begin(), Outside.end());

        for (int i = 0; i < ParentOutsideQuery.size(); ++i)
            items[i] = CopyItems[ParentOutsideQuery[i]];
        for (int i = ParentOutsideQuery.size(); i < items.size(); ++i)
            items[i] = CopyItems[ParentInsideQuery[i]];
    }

    void AddItem(ImportBodyMessage::ImportBodyItem& item)
    {
        item.ShortParentId = (item.parentId.empty()) ? 0 : IdToShort[item.parentId];
        if (!AddId(item))
        {
            Override(item);
            LOG(INFO) << "Item added with id: " << item.id;
            return;
        }
        unsigned long long Short = item.ShortId;
        Node *NewNode = new Node;
        NewNode->ShortId = Short;
        if (item._systemItemType == SystemItemType::FILE) {
            history.Add(item.date_ms, Short, item);
        }
        position[Short] = NewNode;

        if (item.parentId.empty())
        {
            NewNode->parent = nullptr;
            LOG(INFO) << "Item added with id: " << item.id;
            return;
        }

        auto parentNode = position[item.ShortParentId];
        NewNode->parent = parentNode;
        UpdateDate(parentNode, item.date, item.date_ms);

        DecreaseSize(parentNode, -item.size);

        switch (item._systemItemType) {
            case SystemItemType::FOLDER:
                parentNode->childrenFolders[Short] = NewNode;
                break;
            case SystemItemType::FILE:
                parentNode->childrenFiles[Short] = NewNode;
                break;
        }
        LOG(INFO) << "Item added with id: " << item.id;
    }
    void Override(const ImportBodyMessage::ImportBodyItem& item) {
        unsigned long long Short = item.ShortId;
        Node *Node = position[Short];
        if (item._systemItemType == SystemItemType::FILE) {
            history.Add(item.date_ms, Short, item);
        }
        auto OldParentNode = Node->parent;
        if (OldParentNode != nullptr)
            DecreaseSize(OldParentNode, ShortToBody[Short].size);
        auto NewParentNode = position[item.ShortParentId];
        if (NewParentNode != nullptr)
            DecreaseSize(NewParentNode, -item.size);

        UpdateDate(OldParentNode, item.date, item.date_ms);
        UpdateDate(NewParentNode, item.date, item.date_ms);
        if (OldParentNode == NewParentNode)
        {
            ShortToBody[Short] = item;
            return;
        }
        switch(item._systemItemType)
        {
            case SystemItemType::FOLDER:
                if (OldParentNode != nullptr)
                    OldParentNode->childrenFolders.erase(Short);
                if (NewParentNode != nullptr)
                    NewParentNode->childrenFolders[Short] = Node;
                break;
            case SystemItemType::FILE:
                if (OldParentNode != nullptr)
                    OldParentNode->childrenFiles.erase(Short);
                if (NewParentNode != nullptr)
                    NewParentNode->childrenFolders[Short] = Node;
                break;
        }
    }
    GetNodesBodyMessage GetNodes(Node *node)
    {
        LOG(INFO) << "Nodes : current node id = " << ShortToBody[node->ShortId].id
        << " ShortId = " << std::to_string(node->ShortId) << "\nchildren:\n" << PrintChildrenIds(node);
        GetNodesBodyMessage Response;
        Response.body = ShortToBody[node->ShortId];
        vector<GetNodesBodyMessage> children;
        for (auto& it : node->childrenFolders)
            children.push_back(GetNodes(it.second));
        for (auto& it : node->childrenFiles)
            children.push_back(GetNodesBodyMessage{ShortToBody[it.first], vector<GetNodesBodyMessage>()});
        Response.children = children;
        return Response;
    }

    void DeleteItem(Node *node, const string& date, long long ms)
    {
        DecreaseSize(node->parent, ShortToBody[node->ShortId].size);
        UpdateDate(node->parent, date, ms);

        if (node->parent != nullptr) {
            if (ShortToBody[node->ShortId]._systemItemType == SystemItemType::FILE)
                node->parent->childrenFiles.erase(node->ShortId);
            else
                node->parent->childrenFolders.erase(node->ShortId);
        }

        clear(node);
    }

    void DecreaseSize(Node *node, int64_t size)
    {
        while (node != nullptr)
        {
            ShortToBody[node->ShortId].size -= size;
            node = node->parent;
        }
    }

    void UpdateDate(Node *node, const string& date, long long ms)
    {
        while (node != nullptr)
        {
            auto& it = ShortToBody[node->ShortId];
            it.date = date;
            it.date_ms = ms;
            node = node->parent;
            if (it._systemItemType == SystemItemType::FILE)
            {
                history.Add(ms, it.ShortId, it);
            }
        }
    }

    vector<ImportBodyMessage::ImportBodyItem> Update(long long ms)
    {
      history.RemoveMin(ms);
      return history.Update();
    }

    vector<ImportBodyMessage::ImportBodyItem> GetNodeHistory(Node *node, long long start, long long end) const
    {
        vector<ImportBodyMessage::ImportBodyItem> Response;
        //TODO
        return Response;
    }

    MinMap<long long, unsigned long long, ImportBodyMessage::ImportBodyItem> history;
    unordered_map<unsigned long long, ImportBodyMessage::ImportBodyItem> ShortToBody;
    unordered_map<string, unsigned long long> IdToShort;
    unordered_map<unsigned long long, string> ShortToId;
    unordered_map<unsigned long long, Node*> position;

    unsigned long long count = 0;

    bool AddId(ImportBodyMessage::ImportBodyItem& item)
    {
        if (item.ShortId > 0)
            return false;
        item.ShortId = ++count;
        IdToShort[item.id] = count;
        ShortToId[count] = item.id;
        ShortToBody[count] = item;
        return true;
    }

    string PrintChildrenIds(Node *node)
    {
        if (node == nullptr) {
            LOG(ERROR) << "Error in print nodes";
            return "";
        }
        string msg = "Files:\n";
        for (const auto& it : node->childrenFiles)
            msg += ShortToBody[it.first].id + " " + std::to_string(it.first) + "\n";
        msg += "Folders:\n";
        for (const auto& it : node->childrenFolders)
            msg += ShortToBody[it.first].id + " " + std::to_string(it.first) + "\n";
        return msg;
    }

    void Print()
    {
        vector<Node*> roots;
        for (const auto& it : position)
            if (it.second->parent == nullptr)
                roots.push_back(it.second);
        for (auto& it : roots)
        {
            std::cout << "Tree with root " << it->ShortId << "\n";
            Print(it);
            std::cout << "\n";
        }
    }
    void Print(Node *node)
    {
        if (node == nullptr)
            return;
        std :: cout << node->ShortId << "  ";
        for (auto& it : node->childrenFiles)
            Print(it.second);
        for (auto& it : node->childrenFolders)
            Print(it.second);
        std::cout << "\n";
    }

    void ClearMaps()
    {
        ShortToId.clear();
        IdToShort.clear();
        ShortToBody.clear();
        position.clear();
    }

    /*void ReadFromFile(const string& path = migration_folder)
    {
        ClearMaps();

        for (const auto& it : std::filesystem::directory_iterator(path)) {
            json tmp;
            std::ifstream input;
            input.open(it.path());
            input >> tmp;
            const ImportBodyMessage::ImportBodyItem item(tmp);
            auto Short = item.ShortId;
            count = std::max(count, Short);
            ShortToId[Short] = item.id;
            IdToShort[item.id] = Short;
            ShortToBody[Short] = item;
        }

        for (const auto& it : ShortToBody)
        {
            auto NewNode = new Node;
            NewNode->ShortId = it.first;
            position[NewNode->ShortId] = NewNode;
        }

        for (const auto& it : ShortToBody)
        {
            auto& NewNode = position[it.first];
            const auto& par = position.find(it.second.ShortParentId);
            if (par != position.end())
            {
                NewNode->parent = par->second;
                switch (it.second._systemItemType)
                {
                    case SystemItemType::FOLDER:
                        par->second->childrenFolders[it.first] = NewNode;
                        break;
                    case SystemItemType::FILE:
                        par->second->childrenFiles[it.first] = NewNode;
                }
            }
        }
    }*/

    /*void SaveToFileItem(unsigned long long id, const ImportBodyMessage::ImportBodyItem& item)
    {
        std::fstream output;
        string full = migration_folder + std::to_string(id);
        output.open(full, std::fstream::out | std::fstream::trunc);
        output << item.SerializeExpanded();
        output.close();
    }*/

    /*void SaveToFileItem(unsigned long long id)
    {
        ImportBodyMessage::ImportBodyItem item = ShortToBody[id];
        std::fstream output;
        string full = migration_folder + std::to_string(id);
        output.open(full, std::fstream::out | std::fstream::trunc);
        output << item.SerializeExpanded();
        output.close();
    }*/

    std::ofstream migration;

    /*FileSystemTree()
    {
        ReadFromFile();
    }*/

    void clear(Node *node)
    {
        for (auto& it : node->childrenFiles)
            if (it.second != nullptr)
                clear(it.second);
        for (auto& it : node->childrenFolders)
            if (it.second != nullptr)
                clear(it.second);
        unsigned long long Short = node->ShortId;
        string id = ShortToId[Short];
        if (ShortToBody[Short]._systemItemType == SystemItemType::FILE){
            history.Remove(Short);
        }
        IdToShort.erase(id);
        ShortToId.erase(Short);
        position.erase(Short);
        delete node;
    }

    ~FileSystemTree()
    {
        vector<Node*> roots;
        for (const auto& it : position)
            if (it.second->parent == nullptr)
                roots.push_back(it.second);
        for (auto& it: roots)
            clear(it);
    }
};

#endif //SERVER_FILESYSTEMTREE_H
