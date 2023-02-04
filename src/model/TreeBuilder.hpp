#ifndef YANDEX_BACKEND_COPY_TREE_BUILDER_H
#define YANDEX_BACKEND_COPY_TREE_BUILDER_H

#include <nlohmann/json.hpp>

#include "../view/ImportBody.hpp"
#include "../view/GetNodesBody.hpp"

namespace yad_server::model {
    class TreeBuilder {
    public:
        static nlohmann::json build(const std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem> &items, const std::string& root);
    private:
        static inline std::unordered_map<std::string, std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem>>
            getChildren(const std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem> &items);
        static void build(yad_server::view::GetNodesBodyMessage& msg,
                          const yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem& root,
                          const std::unordered_map<std::string, std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem>>& children);
        static int64_t calculateSize(yad_server::view::GetNodesBodyMessage& msg);
        static yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem find_root(const std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem> &items, const std::string& root);
    };
} // end of namespace yad_server::model


#endif //YANDEX_BACKEND_COPY_TREE_BUILDER_H
