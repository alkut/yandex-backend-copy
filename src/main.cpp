#include "logging/init.hpp"
#include "application/BoostApplication.hpp"
#include "model/HttpServer.hpp"
#include "global/ProgramOptions.h"

int main(int argc, char **argv) {
    yad_server::global::ProgramOptions::setInstance(argc, argv);
    yad_server::logging::InitLogging();
    yad_server::application::Application<yad_server::model::HttpServer> app;
    app.run();
    return 0;
}