#include "HttpServer.hpp"
namespace yad_server::model {
        application::Respond yad_server::model::HttpServer::Response(const application::query::Query &query) {
            auto queryExt = validation::QueryExt(query);
            validation::Validator::QueryTypes type;
            try {
                type = validator.GetType(queryExt);
            }
            catch (std::exception &ex) {
                LOG(ERROR) << ex.what();
                return BadRequest;
            }
            switch (type) {
                case validation::Validator::QueryTypes::IMPORT:
                    try {
                        validator.Validate(queryExt, validation::Validator::QueryTypes::IMPORT);
                        auto import_msg = validation::Validator::GetImport(queryExt);
                        ValidateImport(import_msg);
                        file_system.Import(import_msg);
                        return OK;
                    }
                    catch (std::exception &ex) {
                        LOG(ERROR) << ex.what();
                        return BadRequest;
                    }

                case validation::Validator::QueryTypes::DELETE:
                    try {
                        validator.Validate(queryExt, validation::Validator::QueryTypes::DELETE);
                        auto delete_msg = validation::Validator::GetDelete(queryExt);
                        auto ptr = ValidateDelete(delete_msg.id);
                        file_system.Delete(ptr, delete_msg.date, validation::Validator::check_datetime(delete_msg.date));
                        return OK;
                    }
                    catch (NotFoundException &ex) {
                        return NotFound;
                    }
                    catch (std::exception &ex) {
                        LOG(ERROR) << ex.what();
                        return BadRequest;
                    }
                case validation::Validator::QueryTypes::GET_NODES:
                    try {
                        validator.Validate(queryExt, validation::Validator::QueryTypes::GET_NODES);
                        auto nodes_msg = validation::Validator::GetNodes(queryExt);
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
                case validation::Validator::QueryTypes::UPDATE:
                    try {
                        validator.Validate(queryExt, validation::Validator::QueryTypes::UPDATE);
                        auto update_msg = validation::Validator::GetUpdate(queryExt);
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

        long long yad_server::model::HttpServer::ValidateUpdate(const std::string &date) {
            return validation::Validator::check_datetime(date);
        }

        FileSystemTree::Node *HttpServer::ValidateDelete(const std::string &id) const {
            if (file_system.position.find(id) == file_system.position.end())
                throw NotFoundException("try to delete failed.\nitem with id : " + id + "not found");
            return file_system.position.at(id);
        }

        FileSystemTree::Node *HttpServer::ValidateGetNodes(const std::string &id) const {
            return ValidateDelete(id);
        }

        void HttpServer::ValidateImport(view::import_body_message::ImportBodyMessage &msg) {
            auto date_ms = validation::Validator::check_datetime(msg.updateDate);
            if (date_ms < max_date) {
                throw std::invalid_argument("incorrect date");
            }
            max_date = date_ms;
            for (auto &item: msg.Items) {
                item.date = msg.updateDate;
                item.date_ms = date_ms;
            }
            std::unordered_map<std::string, view::import_body_message::ImportBodyMessage::ImportBodyItem> items_from_query;
            for (const auto &item: msg.Items)
                items_from_query[item.id] = item;
            if (items_from_query.size() < msg.Items.size()) {
                throw std::invalid_argument("ids are not unique");
            }

            for (auto &item: msg.Items)
                ValidateImportItem(item, items_from_query);
            TopologySort(msg.Items);
        }

        void HttpServer::ValidateImportItem(view::import_body_message::ImportBodyMessage::ImportBodyItem &item,
                                            const std::unordered_map<std::string, view::import_body_message::ImportBodyMessage::ImportBodyItem> &ids) const {
            auto id = item.id;
            auto parent = item.parentId;
            if (id == parent) {
                throw std::invalid_argument("self loop");
            }
            if (!parent.empty()) {
                if (file_system.position.find(parent) == file_system.position.end()) {
                    // parent inside query
                    if (ids.find(parent) == ids.end())
                        throw std::invalid_argument("no parent found");
                    if (ids.find(parent)->second._systemItemType == view::SystemItemType::FILE)
                        throw std::invalid_argument("parent is file");
                } else {
                    // parent inside filesystem
                    const auto &pos = file_system.position;
                    if (pos.find(parent) == pos.end())
                        throw std::invalid_argument("no parent found");
                    if (pos.find(parent)->second->item._systemItemType == view::SystemItemType::FILE)
                        throw std::invalid_argument("parent is file");
                }
            }
            if (file_system.position.find(id) != file_system.position.end())
                ValidateExistingItem(item);
        }

        void HttpServer::ValidateExistingItem(view::import_body_message::ImportBodyMessage::ImportBodyItem &item) const {
            if (item._systemItemType != file_system.position.find(item.id)->second->item._systemItemType)
                throw std::invalid_argument("try to change type of element");
            if (file_system.IsParent(item.parentId, item.id))
                throw std::invalid_argument("try to make a loop");
        }

        void HttpServer::TopologySort(std::vector<view::import_body_message::ImportBodyMessage::ImportBodyItem> &items) {
            boost::adjacency_list<> G;
            auto CopyItems = items;
            std::unordered_map<std::string, int> IdToIndex;
            for (int i = 0; i < items.size(); ++i) {
                IdToIndex[items[i].id] = i;
            }

            std::unordered_set<int> Outside;
            for (int i = 0; i < items.size(); ++i)
                Outside.insert(i);

            for (int i = 0; i < items.size(); ++i) {
                auto it = IdToIndex.find(items[i].parentId);
                if (it != IdToIndex.end()) {
                    add_edge(i, it->second, G);
                    Outside.erase(i);
                    Outside.erase(it->second);
                }
            }

            std::vector<boost::graph_traits<boost::adjacency_list<>>::vertex_descriptor> c;
            topological_sort(G, std::back_inserter(c));

            std::vector<int> ParentInsideQuery;

            for (const auto &ii: c)
                ParentInsideQuery.push_back(static_cast<int>(ii));

            if (Outside.empty()) {
                for (int i = 0; i < items.size(); ++i)
                    items[i] = CopyItems[ParentInsideQuery[i]];
                return;
            }

            std::vector<int> ParentOutsideQuery(Outside.begin(), Outside.end());

            for (int i = 0; i < ParentOutsideQuery.size(); ++i)
                items[i] = CopyItems[ParentOutsideQuery[i]];
            for (size_t i = ParentOutsideQuery.size(); i < items.size(); ++i)
                items[i] = CopyItems[ParentInsideQuery[i]];
        }

        NotFoundException::NotFoundException(const std::string &msg) {
            LOG(ERROR) << msg;
        }
}