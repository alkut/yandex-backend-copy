#ifndef YANDEX_BACKEND_COPY_FILESYSTEMREPOSITORY_H
#define YANDEX_BACKEND_COPY_FILESYSTEMREPOSITORY_H

#include <string>
#include <exception>
#include <pqxx/pqxx>
#include <pqxx/except>
#include <pqxx/stream_to>

#include "../view/ImportBody.hpp"
#include "../logging/init.hpp"

namespace yad_server::repository {

    class FileSystemRepository {
    public:
        explicit FileSystemRepository(const std::string& connection_string = s);
        void Import(const yad_server::view::import_body_message::ImportBodyMessage &msg);
        void Delete(const std::string &id);
        std::vector<yad_server::view::import_body_message::ImportBodyMessage::ImportBodyItem>
        GetNodes(const std::string &id);
    private:
        const static inline std::string s = "dbname=yashkadb user=alex password=Cc327020 hostaddr=127.0.0.1 port=5432";
        pqxx::connection c;

        static std::string create_type(const yad_server::view::SystemItemType &type);
    };
} // end of namespace yad_server::repository


#endif //YANDEX_BACKEND_COPY_FILESYSTEMREPOSITORY_H
