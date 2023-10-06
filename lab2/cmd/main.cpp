#include "BrzozowskiAutomaton.hpp"
#include "DFA.hpp"
#include <regex>
#include <fstream>
#include "RegexGenerator.hpp"
#include "ProgramOptions.hpp"

int test(std::string s) {
    std::regex regexp("\\(eps\\)");
    BrzozowskiAutomaton automata = BrzozowskiAutomaton(s);
    DFA dfa = DFA(s);
    auto outRegex = automata.convertToRegex();
    outRegex = std::regex_replace(outRegex, regexp, "");
    std::cout << "IN regex: " << s << '\n';
    std::cout << "OUT regex: " << outRegex << "\n";
    regexp = std::regex(outRegex);
    std::string randStr;
    int k = 0;
    for (int i = 0; i < 1000; i++) {
        randStr = dfa.getRandomString();
        if (ProgramOptions::log())
            std::cout << "testing " << randStr << " ";
        if (std::regex_match(randStr, regexp)) {
            if (ProgramOptions::log())
                std::cout << "success " << "\n";
        } else {
            std::cout << "incorrect " << "\n";
            k++;
        }
    }
    std::cout<<"Total errors: "<<k<<"\n"<<"--------------------------"<<'\n';
    return k;
}

int main(int argc, char *argv[]) {
    ProgramOptions::parse(argc, argv);
    std::string_view stringView = ProgramOptions::str();
    int total = 0;
    if (ProgramOptions::need_file()) {
        std::ifstream input_file(stringView.data());
        std::string line;
        while (getline(input_file, line)) {
           total+=test(line.substr(0, line.length() - 1));
        }
        input_file.close();
    } else if (ProgramOptions::use_generator()) {
        std::string arg = stringView.data();
        RegexGenerator r(arg);
        for (auto reg: r.gen()) {
            std::string a = reg.get();
            total+=test(a);
        }
    } else {
        std::string r;
        if (!stringView.empty())
            r = stringView.data();
        else
            r = "";
        total+=test(r);
    }
    std::cout<<"All errors in all tests: " << total<<'\n';
    return 0;
}