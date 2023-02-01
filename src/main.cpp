#include "src/application/BoostApplication.hpp"
#include "src/model/HttpServer.hpp"
int main(int argc, char **argv) {
    yad_server::logging::InitLogging(argv);
    yad_server::application::Application<yad_server::model::HttpServer> app;
    app.run();
    return 0;
}