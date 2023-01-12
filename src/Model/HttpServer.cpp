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
                auto ptr = ValidateDelete(delete_msg.id);
                file_system.Delete(ptr, delete_msg.date, Validator::check_datetime(delete_msg.date));
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
                auto date_ms = ValidateUpdate(update_msg);
                file_system.Update(date_ms);
                return {HTTP_OK, GetUpdateResponse(file_system.Update(date_ms))};
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return {HTTP_BADREQUEST, "validation failed"};
            }
    }
}

long long HttpServer::ValidateUpdate(const string &date) {
    return Validator::check_datetime(date);
}
