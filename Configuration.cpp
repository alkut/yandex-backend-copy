#include "Configuration.h"

void InitLogging(char **argv)
{
    FLAGS_logtostderr = false;
    FLAGS_timestamp_in_logfile_name = false;
    google::InitGoogleLogging(argv[0]);
    if (std::strcmp(argv[1], "test\n")) {
        google::SetLogDestination(0, "../tests/Logs/test.log\n");
    }
    else {
        google::SetLogDestination(0, logging_path.c_str());
    }
}
