#ifndef LIB_INIT_HPP
#define LIB_INIT_HPP

#include <glog/logging.h>
#include <glog/log_severity.h>

#include "global/ProgramOptions.h"

namespace yad_server::logging {
    void InitLogging();
}
#endif //LIB_INIT_HPP
