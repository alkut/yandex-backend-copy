#ifndef YAD_GETNODERESPONSE_H
#define YAD_GETNODERESPONSE_H

#include "Item.h"

class GetNodeResponse
{
public:
    Item item;
    vector<GetNodeResponse> children;
    string Serialize() const;
};

#endif //YAD_GETNODERESPONSE_H
