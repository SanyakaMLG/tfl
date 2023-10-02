#include "BrzozowskiAutomaton.hpp"

int main() {
    BrzozowskiAutomaton automata = BrzozowskiAutomaton("(a*|b*)#a*");
    for (auto el: automata.map) {
        std::cout << el.first << " " << el.second << "\n";
    }
    for (auto el: automata.finalStates) {
        std::cout << el << std::endl;
    }
    for (auto el: automata.transitions) {
        std::cout << el.first.first << " " << el.first.second << " -> " << el.second << "\n";
    }
    return 0;
}