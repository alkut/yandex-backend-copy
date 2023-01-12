#include "src/application/Application.hpp"
#include "src/Model/HttpServer.h"

int main(int argc, char **argv) {
    InitLogging(argv);
    Application<HttpServer> app;
    app.run();
    return 0;
}
