#include "init.hpp"

namespace yad_server::logging {
    void InitLogging() {
        const auto& options = global::ProgramOptions::getInstance();
        FLAGS_logtostderr = false;
        FLAGS_timestamp_in_logfile_name = false;
        google::SetLogDestination(google::ERROR, options.logPath.c_str());
        google::SetLogDestination(google::INFO, options.logPath.c_str());
        google::SetLogDestination(google::WARNING, options.logPath.c_str());
        google::InitGoogleLogging(options.programName.c_str());
    }
}