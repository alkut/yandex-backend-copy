#ifndef YAD_ITEM_H
#define YAD_ITEM_H

#include "Includes.h"

class Item
{
    //Serializable fields
    string id;
    string url;
    string parentId;
    string type;
    int64_t size = 0;
    //Expanded fields
    long long date_ms;
    string date;
    unsigned long long ShortId;
    unsigned long long ShortParentId;
    SystemItemType _systemItemType = SystemItemType::FILE;
};

#endif //YAD_ITEM_H
