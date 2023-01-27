#ifndef YAD_QUERYRESPONDER_HPP
#define YAD_QUERYRESPONDER_HPP
#include "Respond.hpp"
#include "Query.hpp"
namespace yad_server::application {
    class QueryResponder {
    public:
        virtual Respond Response(const application::query::Query &query) = 0;
    };
}
#endif //YAD_QUERYRESPONDER_HPP
