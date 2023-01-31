#include "src/application/Application.hpp"
#include "src/application/async/BoostApplication.hpp"

int main(int argc, char **argv) {
    InitLogging(argv);
    Application2 app;
    Application2::run();
    return 0;
}
