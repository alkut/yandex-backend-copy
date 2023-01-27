#ifndef SERVER_FILE_SYSTEM_TREE_H
#define SERVER_FILE_SYSTEM_TREE_H

#include <utility>
#include <queue>

#include "src/view/GetNodesBody.hpp"
#include "src/logging/init.hpp"
#include "HistoryStorage.hpp"
namespace yad_server::model {
        struct FileSystemTree {
            struct Node {
                Node *parent = nullptr;
                view::ImportBodyMessage::ImportBodyItem item;
                std::unordered_map<std::string, Node *> childrenFiles, childrenFolders;

                explicit Node(const view::ImportBodyMessage::ImportBodyItem &item);

                std::string &id();

                std::string &date();

                long long &date_ms();

                int64_t &size();

                const view::SystemItemType &type() const;
            };

        private:
            HistoryStorage history;
            std::unordered_map<std::string, Node *> position;
        public:
            friend class HttpServer;

            FileSystemTree() = default;

            void Import(view::ImportBodyMessage &msg);

            view::GetNodesBodyMessage GetNodes(Node *node) const;

            void Delete(Node *node, const std::string &date, long long ms);

            std::vector<view::ImportBodyMessage::ImportBodyItem> Update(long long ms);

            ~FileSystemTree();

        private:
            void AddItem(const view::ImportBodyMessage::ImportBodyItem &item);

            void Override(const view::ImportBodyMessage::ImportBodyItem &item);

            static std::vector<Node *> getParents(Node *root);

            static std::vector<Node *> getChildren(Node *root);

            void DestroyNode(Node *node);

            void UnlinkNode(Node *root, const std::string &date, long long ms);

            void UnlinkNode(Node *root);

            static void LinkNode(Node *root, Node *parent, const std::string &date, long long ms);

            static void LinkNode(Node *root, Node *parent);

            bool IsParent(const std::string &node, const std::string &parent) const;
        };
    }
#endif //SERVER_FILE_SYSTEM_TREE_H
