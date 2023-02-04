#ifndef LIB_VALIDATOR_HPP
#define LIB_VALIDATOR_HPP

#include <boost/date_time/posix_time/posix_time.hpp>

#include "QueryExt.hpp"
#include "DeleteItem.hpp"
#include "../view/ImportBody.hpp"
#include "../logging/init.hpp"

namespace yad_server::validation {
    class Validator {
    public:
        static view::import_body_message::ImportBodyMessage GetImport(const QueryExt& query);
        static DeleteItem GetDelete(const QueryExt& query);
        static std::string GetNodes(const QueryExt& query);
        static std::string check_datetime(const std::string& date);
    };
} // end of namespace yad_server::validation
#endif //LIB_VALIDATOR_HPP
