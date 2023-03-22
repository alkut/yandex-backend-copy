#include "init.hpp"

namespace yad_server::logging {
    void InitLogging() {
        const auto& options = global::ProgramOptions::getInstance();
        FLAGS_logtostderr = false;
        FLAGS_timestamp_in_logfile_name = false;
        auto fullPath = options.logPath + logName;
        google::SetLogDestination(google::INFO, fullPath.c_str());
        google::InitGoogleLogging(options.programName.c_str());
    }
}