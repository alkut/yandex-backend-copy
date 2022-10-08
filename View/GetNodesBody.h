#ifndef SERVER_GETNODESBODY_H
#define SERVER_GETNODESBODY_H

#include "ImportBody.h"
#include "Includes.h"

struct GetNodesBodyMessage
{
    json SerializeDump() const
    {
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

    string Serialize() const
    {
        return SerializeDump().dump(2);
    }

    ImportBodyMessage::ImportBodyItem body;

    vector<GetNodesBodyMessage> children;
};

#endif //SERVER_GETNODESBODY_H
