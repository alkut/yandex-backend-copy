#ifndef SERVER_GET_NODES_BODY_H
#define SERVER_GET_NODES_BODY_H

#include "ImportBody.hpp"
namespace yad_server::view {
    struct GetNodesBodyMessage {
        [[nodiscard]] nlohmann::json SerializeDump() const;

        [[nodiscard]] std::string Serialize() const;

        ImportBodyMessage::ImportBodyItem body;
        std::vector<GetNodesBodyMessage> children;
    };
}

#endif //SERVER_GET_NODES_BODY_H
