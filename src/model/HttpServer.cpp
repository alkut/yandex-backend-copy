#include "HttpServer.hpp"

Respond HttpServer::Response(const Query &query) {
    auto queryExt = QueryExt(query);
    Validator::QueryTypes type;
    try {
        type = validator.GetType(queryExt);
    }
    catch (std::exception &ex) {
        LOG(ERROR) << ex.what();
        return BadRequest;
    }
    switch (type) {
        case Validator::QueryTypes::IMPORT:
            try {
                validator.Validate(queryExt, Validator::QueryTypes::IMPORT);
                auto import_msg = Validator::GetImport(queryExt);
                ValidateImport(import_msg);
                file_system.Import(import_msg);
                return OK;
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return BadRequest;
            }

        case Validator::QueryTypes::DELETE:
            try {
                validator.Validate(queryExt, Validator::QueryTypes::DELETE);
                auto delete_msg = Validator::GetDelete(queryExt);
                auto ptr = ValidateDelete(delete_msg.id);
                file_system.Delete(ptr, delete_msg.date, Validator::check_datetime(delete_msg.date));
                return OK;
            }
            catch (NotFoundException &ex) {
                return NotFound;
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return BadRequest;
            }
        case Validator::QueryTypes::GET_NODES:
            try {
                validator.Validate(queryExt, Validator::QueryTypes::GET_NODES);
                auto nodes_msg = Validator::GetNodes(queryExt);
                auto ptr = ValidateGetNodes(nodes_msg);
                auto result = file_system.GetNodes(ptr);
                return {HTTP_OK, result.Serialize()};
            }
            catch (NotFoundException &ex) {
                return NotFound;
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return BadRequest;
            }
        case Validator::QueryTypes::UPDATE:
            try {
                validator.Validate(queryExt, Validator::QueryTypes::UPDATE);
                auto update_msg = Validator::GetUpdate(queryExt);
                auto date_ms = ValidateUpdate(update_msg);
                file_system.Update(date_ms);
                return OK;
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return BadRequest;
            }
    }
    return BadRequest;
}

long long HttpServer::ValidateUpdate(const string &date) {
    return Validator::check_datetime(date);
}

FileSystemTree::Node *HttpServer::ValidateDelete(const string &id) const {
    if (file_system.position.find(id) == file_system.position.end())
        throw NotFoundException("try to delete failed.\nitem with id : " + id + "not found");
    return file_system.position.at(id);
}

FileSystemTree::Node *HttpServer::ValidateGetNodes(const string &id) const {
    return ValidateDelete(id);
}

void HttpServer::ValidateImport(ImportBodyMessage &msg) const {
    std::unordered_set<std::string> ids_from_query;
    for (const auto& item: msg.Items)
        ids_from_query.insert(item.id);
    if (ids_from_query.size() < msg.Items.size()) {
        throw std::invalid_argument("ids are not unique");
    }

    for (auto& item : msg.Items)
        ValidateImportItem(item, ids_from_query);
}

void HttpServer::ValidateImportItem(ImportBodyMessage::ImportBodyItem &item, const std::unordered_set<std::string> &ids) const {
    auto id = item.id;
    auto parent = item.parentId;
    if (id == parent) {
        throw std::invalid_argument("");
    }
    if (!parent.empty() && file_system.position.find(parent) == file_system.position.end() && ids.find(parent) == ids.end()) {
        throw std::invalid_argument("");
    }

}

NotFoundException::NotFoundException(const string &msg) {
    LOG(ERROR) << msg;
}
