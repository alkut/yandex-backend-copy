#ifndef SERVER_GET_NODES_BODY_H
#define SERVER_GET_NODES_BODY_H

#include "ImportBody.h"

template<class T>
using vector = std::vector<T>;

struct GetNodesBodyMessage
{
    [[nodiscard]] json SerializeDump() const {
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
    [[nodiscard]] string Serialize() const {
        return SerializeDump().dump(2);
    }

    ImportBodyMessage::ImportBodyItem body;
    vector<GetNodesBodyMessage> children;
};

#endif //SERVER_GET_NODES_BODY_H
