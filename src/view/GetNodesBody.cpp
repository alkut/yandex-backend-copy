#include "GetNodesBody.hpp"

namespace yad_server::view {
    std::string GetNodesBodyMessage::Serialize() const {
        return SerializeDump().dump(2);
    }

    nlohmann::json GetNodesBodyMessage::SerializeDump() const {
        nlohmann::json j = body.SerializeDump();
        if (!children.empty()) {
            nlohmann::json arr;
            for (const auto &it: children)
                arr.push_back(it.SerializeDump());
            j["children"] = arr;
        } else
            j["children"] = (body._systemItemType == SystemItemType::FILE) ? NullJson : EmptyJson;

        return j;
    }
}
