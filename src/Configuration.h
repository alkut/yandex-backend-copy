#ifndef SERVER_CONFIGURATION_H
#define SERVER_CONFIGURATION_H

#include "Includes.h"
#include <glog/logging.h>

const char SrvAddress[] = "127.0.0.1";
const std::uint16_t SrvPort = 8080;

const string migration_path = "../Migrations/migration.txt";
const string logging_path = "../Logs/server.log";

void InitLogging(char **argv);

#endif //SERVER_CONFIGURATION_H
