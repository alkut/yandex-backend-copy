#include "FileSystemTree.hpp"
namespace yad_server::model {
        void FileSystemTree::Import(view::ImportBodyMessage &msg) {
            for (const auto &item: msg.Items)
                AddItem(item);
        }

        view::GetNodesBodyMessage FileSystemTree::GetNodes(Node *node) const {
            view::GetNodesBodyMessage Response;
            Response.body = node->item;
            std::vector<view::GetNodesBodyMessage> children;
            for (const auto&[id, ptr]: node->childrenFolders)
                children.push_back(GetNodes(ptr));
            for (const auto&[id, ptr]: node->childrenFiles)
                children.push_back(view::GetNodesBodyMessage{ptr->item, std::vector<view::GetNodesBodyMessage>()});
            Response.children = children;
            return Response;
        }

        void FileSystemTree::Delete(Node *node, const std::string &date, long long ms) {
            UnlinkNode(node, date, ms);
            DestroyNode(node);
        }

        std::vector<view::ImportBodyMessage::ImportBodyItem> FileSystemTree::Update(long long ms) {
            history.DeleteOld(ms);
            return history.GetAll();
        }

        void FileSystemTree::AddItem(const view::ImportBodyMessage::ImportBodyItem &item) {
            if (position.find(item.id) != position.end()) {
                Override(item);
                LOG(INFO) << "Item added with id: " << item.id;
                return;
            }
            history.Add(item);
            Node *NewNode = new Node(item);
            position[item.id] = NewNode;
            auto parentNode = item.parentId.empty() ? nullptr :
                              position[item.parentId];
            LinkNode(NewNode, parentNode);
            LOG(INFO) << "Item added with id: " << item.id;
        }

        void FileSystemTree::Override(const view::ImportBodyMessage::ImportBodyItem &item) {
            history.Add(item);
            Node *Node = position[item.id];
            auto it = position.find(item.id);
            auto parentNode = (it != position.end()) ? it->second : nullptr;
            if (parentNode != nullptr)
                parentNode = parentNode->parent;
            UnlinkNode(Node);
            Node->item = item;
            LinkNode(Node, parentNode);
        }

        FileSystemTree::~FileSystemTree() {
            std::vector<Node *> roots;
            for (const auto&[id, ptr]: position)
                if (ptr->parent == nullptr)
                    roots.push_back(ptr);
            for (auto &it: roots)
                DestroyNode(it);
        }

///get all parents exclude root itself
        std::vector<FileSystemTree::Node *> FileSystemTree::getParents(FileSystemTree::Node *root) {
            std::vector<FileSystemTree::Node *> ans;
            root = root->parent;
            while (root) {
                ans.push_back(root);
                root = root->parent;
            }
            return ans;
        }

///get all children include root itself
        std::vector<FileSystemTree::Node *> FileSystemTree::getChildren(FileSystemTree::Node *root) {
            std::vector<FileSystemTree::Node *> ans;
            std::queue<FileSystemTree::Node *> queue;
            queue.push(root);
            ans.push_back(root);
            while (!queue.empty()) {
                auto top = queue.back();
                for (const auto&[name, ptr]: top->childrenFiles) {
                    ans.push_back(ptr);
                }
                for (const auto&[name, ptr]: top->childrenFolders) {
                    ans.push_back(ptr);
                    queue.push(ptr);
                }
                queue.pop();
            }
            return ans;
        }

        void FileSystemTree::DestroyNode(Node *node) {
            auto children = getChildren(node);
            for (auto &ptr: children) {
                history.Remove(ptr->item);
                position.erase(ptr->id());
                delete ptr;
            }
        }

        void FileSystemTree::UnlinkNode(FileSystemTree::Node *root, const std::string &date, long long ms) {
            if (root == nullptr)
                return;
            auto parents = getParents(root);
            for (auto &ptr: parents) {
                ptr->size() -= root->size();
                ptr->date() = date;
                ptr->date_ms() = ms;
                history.Add(ptr->item);
            }
            if (root->parent == nullptr)
                return;
            if (root->type() == view::SystemItemType::FILE)
                root->parent->childrenFiles.erase(root->id());
            else
                root->parent->childrenFolders.erase(root->id());
        }

        void FileSystemTree::UnlinkNode(FileSystemTree::Node *root) {
            if (root == nullptr)
                return;
            UnlinkNode(root, root->date(), root->date_ms());
        }

        void FileSystemTree::LinkNode(FileSystemTree::Node *root, FileSystemTree::Node *parent, const std::string &date,
                                      long long ms) {
            if (root == nullptr || parent == nullptr)
                return;
            root->parent = parent;
            if (root->type() == view::SystemItemType::FILE)
                parent->childrenFiles[root->id()] = root;
            else
                parent->childrenFolders[root->id()] = root;

            auto parents = getParents(root);
            for (auto &ptr: parents) {
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

        bool FileSystemTree::IsParent(const std::string &node_id, const std::string &parent_id) const {
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

        FileSystemTree::Node::Node(const view::ImportBodyMessage::ImportBodyItem &item) : item(item) {}

        std::string &FileSystemTree::Node::id() {
            return item.id;
        }

        std::string &FileSystemTree::Node::date() {
            return item.date;
        }

        long long &FileSystemTree::Node::date_ms() {
            return item.date_ms;
        }

        int64_t &FileSystemTree::Node::size() {
            return item.size;
        }

        const view::SystemItemType &FileSystemTree::Node::type() const {
            return item._systemItemType;
        }
}