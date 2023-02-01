#include "src/application/async/BoostApplication.hpp"
#include "src/model/HttpServer.hpp"
int main(int argc, char **argv) {
    yad_server::logging::InitLogging(argv);
//    yad_server::application::Application2<yad_server::model::HttpServer> app;
//    app.run();
    return 0;
}