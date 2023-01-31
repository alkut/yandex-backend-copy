#include "src/application/async/BoostApplication.hpp"

query::Query MakeQuery(const std::string& Uri, const std::string& body) {
    std::vector<std::string> splittedUri;
    boost::split(splittedUri, Uri, boost::is_any_of("?"));
    if (splittedUri.size() == 1) {
        return {Uri, {}, body};
    }
    if (splittedUri.size() > 2) {
        LOG(ERROR) << "invalid uri: ";
        throw std::invalid_argument( "invalid uri.");
    }
    std::unordered_map<std::string, std::string> params_um;
    std::vector<std::string> params_vector;
    boost::split(params_vector, splittedUri[1], boost::is_any_of("&"));
    for (const auto &params: params_vector) {
        std::vector<std::string> key_value;
        boost::split(key_value, params, boost::is_any_of("="));
        if (key_value.size() != 2)
        {
            LOG(ERROR) << "invalid pair of parameters in query: " << params;
            throw std::invalid_argument( "invalid pair of parameters in query.");

        }
        params_um[key_value[0]] = key_value[1];
    }
    return {splittedUri[0], params_um, body};
}
