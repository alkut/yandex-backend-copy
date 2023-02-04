#include "TreeBuilder.hpp"

namespace yad_server::model {
    nlohmann::json TreeBuilder::build(
            const std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem> &items,
            const std::string& root) {
        yad_server::view::GetNodesBodyMessage ans;
        auto children = getChildren(items);
        build(ans, find_root(items, root), children);
        calculateSize(ans);
        return ans.SerializeDump();
    }

    std::unordered_map<std::string, std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem>>
    TreeBuilder::getChildren(
            const std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem> &items) {
        std::unordered_map<std::string, std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem>> children;
        for (const auto& item: items)
            children[item.parentId].push_back(item);
        return children;
    }

    void TreeBuilder::build(view::GetNodesBodyMessage &msg,
                            const view::import_body_message::ImportBodyMessage::ImportBodyItem &root,
                            const std::unordered_map<std::string, std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem>> &children) {
        msg.body = root;
        auto child = children.find(root.id)->second;
        for (const auto& item: child) {
            msg.children.push_back({});
            build(msg.children.back(), item, children);
        }
    }

    int64_t TreeBuilder::calculateSize(view::GetNodesBodyMessage &msg) {
        int64_t ans = 0;
        for (auto& child: msg.children)
            ans += calculateSize(child);
        msg.body.size = ans;
        return ans;
    }

    yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem TreeBuilder::find_root(
            const std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem> &items,
            const std::string &root) {
        for (const auto& item: items)
            if (item.id == root)
                return item;
        throw std::invalid_argument("no root item was found");
    }
}
