#ifndef YAD_UPDATERESPONSE_H
#define YAD_UPDATERESPONSE_H

#include "Item.h"
#include "Includes.h"

class UpdateResponse
{
public:
    vector<Item> items;
    string Serialize() const;
};

#endif //YAD_UPDATERESPONSE_H
