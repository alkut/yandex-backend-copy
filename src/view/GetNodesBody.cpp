#include "GetNodesBody.h"

string GetNodesBodyMessage::Serialize() const {
    return SerializeDump().dump(2);
}

json GetNodesBodyMessage::SerializeDump() const {
    json j = body.SerializeDump();
    if (!children.empty())
    {
        json arr;
        for (const auto& it : children)
            arr.push_back(it.SerializeDump());
        j["children"] = arr;
    }
    else
        j["children"] = (body._systemItemType == SystemItemType::FILE) ? NullJson : EmptyJson;

    return j;
}
