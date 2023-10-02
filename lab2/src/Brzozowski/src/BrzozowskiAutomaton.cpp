#ifndef LAB2_BRZOZOWSKIAUTOMATON_CPP
#define LAB2_BRZOZOWSKIAUTOMATON_CPP

#include <string>
#include "BrzozowskiAutomaton.hpp"
#include "Derivative.hpp"


const int EMPTY(-1);
const int EPS(0);
const int WORD(1);
const int OPEN_BRACKET(2);
const int CLOSE_BRACKET(3);
const int ALTERNATIVE(4);
const int SHUFFLE(5);
const int CONCAT(6);
const int KLINI(7);



std::string BrzozowskiAutomaton::findFirstUncompleted() {
    for (auto elem: completeStates) {
        if (!elem.second) return elem.first;
    }
    return "";
}

void BrzozowskiAutomaton::addDerivativeBySymbol(Node *t, char c,
                                                int& curState, int oldState) {
    Regex regex(t);
    auto newTree = regex.der( c);
    std::string newRegex = newTree.get();
    if (!this->map.contains(newRegex)) {
        map[newRegex] = curState;
        transitions.emplace_back(std::make_pair(oldState, c), curState);
        completeStates[newRegex] = false;
        if (containsEPS(regex.getTree())) finalStates.push_back(curState);
        curState++;
    } else {
        int toState = map[newRegex];
        transitions.push_back(std::make_pair(std::make_pair(oldState, c), toState));
    }
}

void BrzozowskiAutomaton::addDerivativeByAlphabet(std::string regex,
                                                  int &curState) {
    auto tree = Regex(regex).getTree();
    for (auto c: this->alphabet) {
        addDerivativeBySymbol(tree, c, curState, map[regex]);
    }
    completeStates[regex] = true;
}



BrzozowskiAutomaton::BrzozowskiAutomaton(std::string initialRegex) {
    for (auto c: initialRegex) {
        if (std::isalpha(c)) this->alphabet.insert(c);
    }
    initialState = 0;
    map[initialRegex] = 0;
    Regex initialRegex1 = Regex(initialRegex);
    if (containsEPS(initialRegex1.getTree())) finalStates.push_back(0);
    completeStates[initialRegex] = false;
    int curState = 1;
    while (!findFirstUncompleted().empty()) {
        auto regex = findFirstUncompleted();
        addDerivativeByAlphabet(regex, curState);
    }
}

#endif