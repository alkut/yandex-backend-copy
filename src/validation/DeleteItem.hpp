#ifndef LIB_DELETEITEM_HPP
#define LIB_DELETEITEM_HPP


#include <string>
namespace yad_server::validation {
    struct DeleteItem {
        std::string id;
        std::string date;
    };
}
#endif //LIB_DELETEITEM_HPP
