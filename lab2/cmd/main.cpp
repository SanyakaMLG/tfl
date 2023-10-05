#include "BrzozowskiAutomaton.hpp"
#include "DFA.hpp"
#include <regex>

int main() {
    BrzozowskiAutomaton automata = BrzozowskiAutomaton("(a*|b*)#a*b");
    DFA dfa = DFA("(a*|b*)#a*b");
//    for (auto arr: dfa.reachabilityMatrix) {
//        for (auto elem: arr) {
//            std::cout << elem << " ";
//        }
//        std::cout << "\n";
//    }
//    for (auto trans: dfa.transitions) {
//        std::cout << trans.first.first << " -> " << trans.first.second << ": ";
//        for (auto c: trans.second) {
//            std::cout << c << " ";
//        }
//        std::cout << "\n";
//    }
    auto outRegex = automata.convertToRegex();
    std::regex regexp("\\(eps\\)");
    outRegex = std::regex_replace(outRegex, regexp, "");
    regexp = std::regex(outRegex);
    std::cout << outRegex << "\n";
    for (int i = 0; i < 1000; i++) {
        if (std::regex_match(dfa.getRandomString(), regexp)) {
        } else {
            std::cout << "incorrect\n";
        }
    }
    return 0;
}