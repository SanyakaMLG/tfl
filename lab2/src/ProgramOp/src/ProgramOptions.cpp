#include "ProgramOptions.hpp"

#include <filesystem>
#include <stdexcept>
#include <vector>

namespace {

    std::string_view _string;
    bool _need_file = false;
    bool _use_genererator = false;
    bool _log = false;
}  // namespace
void
ProgramOptions::parse(int argc, char *argv[]) {
    const std::vector<std::string_view> args(argv + 1, argv + argc);

    for (const auto &arg: args) {

            if (arg == "-f" || arg == "--file") {
                if (_need_file) {
                    throw std::runtime_error("regex: cannot use -f/--file parameter twice!");
                }
                _need_file = true;
                continue;
            }

            if (arg == "-l" || arg == "--log") {
                if (_log) {
                    throw std::runtime_error("regex: cannot use -l/--log parameter twice!");
                }
                _log = true;
                continue;
            }

            if (arg == "-g" || arg == "--use-generator") {
                if (_use_genererator || _need_file) {
                    throw std::runtime_error("regex: cannot use -g/--use-generator parameter twice!");
                }
                _use_genererator = true;
                continue;
            }

        if ((_need_file || _use_genererator) && !std::filesystem::exists(arg)) {
            throw std::runtime_error(std::string("regex: ") + std::string(arg) + ": No such file or directory");
        }
        _string = std::string_view(arg);
    }
}

const std::string_view &ProgramOptions::str() {
    return _string;
}

bool ProgramOptions::need_file() {
    return _need_file;
}

bool ProgramOptions::use_generator() {
    return _use_genererator;
}

bool ProgramOptions::log() {
    return _log;
}