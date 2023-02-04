#include "FileSystemRepository.hpp"

namespace yad_server::repository {
    FileSystemRepository::FileSystemRepository(const std::string &connection_string) {
        c = pqxx::connection(connection_string);
    }

    void FileSystemRepository::Import(const yad_server::view::import_body_message::ImportBodyMessage &msg) {
        pqxx::work txn{c};

        pqxx::stream_to stream = pqxx::stream_to::raw_table(txn, "public.import",
                                                            "id, url, size, type, update_date, parent_id");
        std::vector<std::tuple<std::string, std::string, int64_t, std::string, std::string, std::string>> vector_of_vectors;
        for (const auto &item: msg.Items) {
            vector_of_vectors.emplace_back(item.id, item.url, item.size, create_type(item._systemItemType),
                                           item.date, item.parentId);
        }
        for (const auto &row: vector_of_vectors) {
            stream << row;
        }
        try {
            stream.complete();
            txn.commit();
        }
        catch (const pqxx::sql_error &e) {
            LOG(ERROR) << e.what() << std::endl;
            throw std::invalid_argument("error in import validation");
        }
    }

    void FileSystemRepository::Delete(const std::string &id, const std::string& date) {
        pqxx::work txn{c};

        pqxx::stream_to stream = pqxx::stream_to::raw_table(txn, "public.delete", "id, update");
        stream << std::make_tuple(id, date);
        try {
            stream.complete();
            txn.commit();
        }
        catch (const pqxx::sql_error &e) {
            LOG(ERROR) << e.what() << std::endl;
            throw std::invalid_argument("error in delete validation");
        }
    }

    std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem>
    FileSystemRepository::GetNodes(const std::string &id) {
        ///TODO
        return {};
    }

    std::string FileSystemRepository::create_type(const yad_server::view::SystemItemType &type) {
        if (type == yad_server::view::SystemItemType::FOLDER)
            return "'folder'::type";
        return "'file'::type";
    }
} // end of namespace yad_server::repository
