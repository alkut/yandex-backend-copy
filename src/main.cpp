#include "src/application/async/BoostApplication.hpp"
#include "src/model/HttpServer.hpp"

int main(int argc, char **argv) {
    InitLogging(argv);
    Application2<HttpServer> app;
    app.run();
    return 0;
}
