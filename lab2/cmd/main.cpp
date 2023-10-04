#include "BrzozowskiAutomaton.hpp"
#include "RegexGenerator.hpp"
int main() {
    RegexGenerator g(4,3,30);
    auto regexes = g.gen(12);
    for(auto t: regexes){
        std::cout << t.get() <<'\n';
    }
   // BrzozowskiAutomaton automata = BrzozowskiAutomaton("a*e*ed#eb");
   // automata.convertToRegex();
//    for (auto el: automata.map) {
//        std::cout << el.first << " " << el.second << "\n";
//    }
//    for (auto el: automata.finalStates) {
//        std::cout << el << std::endl;
//    }
//    for (auto el: automata.transitions) {
//        std::cout << el.first.first << " " << el.first.second << " -> " << el.second << "\n";
//    }
    //auto regex = automata.convertToRegex();
    //std::cout << regex;
    return 0;
}