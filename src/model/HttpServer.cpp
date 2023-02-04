#include "HttpServer.hpp"

namespace yad_server::model {
    application::Respond HttpServer::Response(const application::query::Query &query) {
        auto ext = validation::QueryExt(query);
        return actions.find(ext.parsed_url[0])->second(ext);
    }

    application::Respond HttpServer::HandleImport(const validation::QueryExt &query) {
        try {
            repository.Import(validation::Validator::GetImport(query));
        }
        catch (const std::exception& ex) {
            LOG(ERROR) << ex.what();
            return BadRequest;
        }
        return OK;
    }

    application::Respond HttpServer::HandleDelete(const validation::QueryExt &query) {
        try {
            auto delete_item = validation::Validator::GetDelete(query);
            repository.Delete(delete_item.id, delete_item.date);
        }
        catch (const std::out_of_range& ex) {
            LOG(ERROR) << ex.what();
            return NotFound;
        }
        catch (const std::invalid_argument& ex) {
            LOG(ERROR) << ex.what();
            return BadRequest;
        }
        return OK;
    }

    application::Respond HttpServer::HandleGetNodes(const validation::QueryExt &query) {
        try {
            auto root = validation::Validator::GetNodes(query);
            return {200, TreeBuilder::build(repository.GetNodes(root), root).dump(2)};
        }
        catch (const std::out_of_range& ex) {
            LOG(ERROR) << ex.what();
            return NotFound;
        }
        catch (const std::invalid_argument& ex) {
            LOG(ERROR) << ex.what();
            return BadRequest;
        }
    }
}