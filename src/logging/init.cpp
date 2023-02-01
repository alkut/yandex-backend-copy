#include "init.hpp"
namespace yad_server::logging {
    void InitLogging(char **argv) {
        FLAGS_logtostderr = false;
        FLAGS_timestamp_in_logfile_name = false;
        google::InitGoogleLogging(argv[0]);
    }
}