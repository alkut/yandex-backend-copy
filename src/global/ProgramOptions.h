#ifndef MAIN_PROGRAM_OPTIONS_H
#define MAIN_PROGRAM_OPTIONS_H

#include <boost/program_options.hpp>

namespace yad_server::global {

    class ProgramOptions {
    public:
        enum Profile: int {
            DEV = 0,
            TEST = 1,
            PROD = 2
        };

        Profile profile = PROD;
        std::string logPath = "../logs";
        std::string hostAddress = "0.0.0.0";
        std::string programName = "main";
        short unsigned int portNumber = 8080;

        static ProgramOptions& getInstance();

        static void setInstance(int argc, char **argv);

    private:
        static ProgramOptions *instance;
        ProgramOptions(int argc, char **argv);

        static Profile fromString(const std::string&);
    };
} // end of namespace yad_server::global


#endif //MAIN_PROGRAM_OPTIONS_H
