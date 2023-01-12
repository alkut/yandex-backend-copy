#ifndef SERVER_GET_NODES_BODY_H
#define SERVER_GET_NODES_BODY_H

#include "ImportBody.hpp"

template<class T>
using vector = std::vector<T>;

struct GetNodesBodyMessage {
    [[nodiscard]] json SerializeDump() const;
    [[nodiscard]] string Serialize() const;
    ImportBodyMessage::ImportBodyItem body;
    vector<GetNodesBodyMessage> children;
};

#endif //SERVER_GET_NODES_BODY_H
