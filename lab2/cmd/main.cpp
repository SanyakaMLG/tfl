#include "BrzozowskiAutomaton.hpp"
int main() {
    BrzozowskiAutomaton automata = BrzozowskiAutomaton("(a*|b*)#a*");
    return 0;
}