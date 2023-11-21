#ifndef LAB2_PROGRAMOPTIONS_HPP
#define LAB2_PROGRAMOPTIONS_HPP

#include <string_view>
#include <vector>

namespace ProgramOptions {

    void parse(int argc, char* argv[]);

    const std::string_view& str();
    bool need_file();
    bool use_generator();
    bool log();

}

#endif //LAB2_PROGRAMOPTIONS_HPP
