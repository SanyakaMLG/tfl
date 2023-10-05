#include "BrzozowskiAutomaton.hpp"
#include "DFA.hpp"

int main() {
    BrzozowskiAutomaton automata = BrzozowskiAutomaton("(a*|b*)#a*");
    DFA dfa = DFA("(a*|b*)#a*");
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
    auto regex = automata.convertToRegex();
    for (int i = 0; i < 20; i++) {
        std::cout << dfa.getRandomString() << "\n";
    }
    std::cout << regex;
    return 0;
}