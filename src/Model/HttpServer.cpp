#include "HttpServer.h"

Respond HttpServer::Response(const Query &query) {
    auto queryExt = QueryExt(query);
    auto type = validator.GetType(queryExt);
    auto respond = Respond();
    switch (type) {
        case Validator::QueryTypes::IMPORT:
            try {
                validator.Validate(queryExt, Validator::QueryTypes::IMPORT);
                auto import_msg = Validator::GetImport(queryExt);
                file_system.Import(import_msg);
                return {HTTP_OK, ""};
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return {HTTP_BADREQUEST, "validation failed"};
            }

        case Validator::QueryTypes::DELETE:
            try {
                validator.Validate(queryExt, Validator::QueryTypes::DELETE);
                auto delete_msg = Validator::GetDelete(queryExt);
                //file_system.Delete(delete_msg);
                return {HTTP_OK, ""};
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return {HTTP_BADREQUEST, "validation failed"};
            }
        case Validator::QueryTypes::GET_NODES:
            try {
                validator.Validate(queryExt, Validator::QueryTypes::GET_NODES);
                auto nodes_msg = Validator::GetNodes(queryExt);
                //file_system.GetNodes(nodes_msg);
                return {HTTP_OK, ""};
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return {HTTP_BADREQUEST, "validation failed"};
            }
        case Validator::QueryTypes::UPDATE:
            try {
                validator.Validate(queryExt, Validator::QueryTypes::UPDATE);
                auto update_msg = Validator::GetUpdate(queryExt);
                //file_system.Update(update_msg);
                return {HTTP_OK, ""};
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return {HTTP_BADREQUEST, "validation failed"};
            }
    }
}
