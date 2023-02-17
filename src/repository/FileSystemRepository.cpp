#include "FileSystemRepository.hpp"

namespace yad_server::repository {
    FileSystemRepository::FileSystemRepository(const std::string &connection_string) : c(connection_string) {
        c.prepare(
                "get_nodes",
                "    WITH RECURSIVE children_of (id, parent_id) AS (\n"
                "        SELECT\n"
                "            connection.id AS id, connection.parent_id AS parent_id\n"
                "        FROM\n"
                "            connection\n"
                "        WHERE\n"
                "            connection.id = $1\n"
                "        UNION\n"
                "        SELECT\n"
                "            connection.id AS id, connection.parent_id AS parent_id\n"
                "        FROM\n"
                "            connection\n"
                "        INNER JOIN\n"
                "            children_of\n"
                "        ON\n"
                "            connection.parent_id = children_of.id\n"
                "    )\n"
                "    SELECT\n"
                "        item.id AS id, item.url AS url, item.size AS size,\n"
                "        item.type AS type, item.update AS update, children_of.parent_id AS parent_id\n"
                "    FROM\n"
                "        children_of\n"
                "    JOIN\n"
                "        item\n"
                "    ON\n"
                "        item.id = children_of.id\n");
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
            throw std::out_of_range("error in delete validation");
        }
    }

    std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem>
    FileSystemRepository::GetNodes(const std::string &id) {
        pqxx::work txn(c);
        auto res = txn.exec_prepared("get_nodes", id);
        txn.commit();
        std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem> ans;
        for (const auto & x : res)
            ans.emplace_back(to_string(x.at("id")), to_string(x.at("url")), to_string(x.at("parent_id")),
                             to_string(x.at("type")), x.at("size").as<int64_t>(), to_string(x.at("update")));
        return ans;
    }

    std::string FileSystemRepository::create_type(const yad_server::view::SystemItemType &type) {
        if (type == yad_server::view::SystemItemType::FOLDER)
            return "'folder'::type";
        return "'file'::type";
    }
} // end of namespace yad_server::repository
