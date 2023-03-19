#include "ProgramOptions.h"

namespace yad_server::global {

    ProgramOptions* ProgramOptions::instance = nullptr;

    ProgramOptions &ProgramOptions::getInstance() {
        if (instance == nullptr)
            throw std::runtime_error("try to get instance before it was set");
        return *instance;
    }

    void ProgramOptions::setInstance(int argc, char **argv) {
        if (instance != nullptr)
            return;
        instance = new ProgramOptions(argc, argv);
    }

    ProgramOptions::ProgramOptions(int argc, char **argv) {
        boost::program_options::options_description desc ("Allowed options");
        std::string profile_str, logs, host;
        short unsigned int port;
        desc.add_options ()
                ("profile", boost::program_options::value(&profile_str), "profile (PROD/TEST/DEV)")
                ("logPath", boost::program_options::value(&logs), "Path for logs")
                ("address", boost::program_options::value(&host))
                ("port", boost::program_options::value(&port));

        boost::program_options::variables_map vm;
        boost::program_options::store (boost::program_options::command_line_parser (argc, argv).options (desc).run (), vm);
        boost::program_options::notify (vm);
        if (vm.contains("profile")) {
            profile = fromString(vm["profile"].as<std::string>());
        }

        if (vm.contains("logPath")) {
            logPath = vm["logPath"].as<std::string>();
        }

        if (vm.contains("address")) {
            hostAddress = vm["address"].as<std::string>();
        }

        if (vm.contains("port")) {
            portNumber = vm["port"].as<short unsigned int>();
        }

        programName = argv[0];
    }

    ProgramOptions::Profile ProgramOptions::fromString(const std::string &str) {
        if (str == "DEV")
            return DEV;
        if (str == "TEST")
            return TEST;
        return PROD;
    }

}
