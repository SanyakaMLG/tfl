//
// Created by aleks on 01.10.2023.
//

#include <string>
#include "BrzozowskiAutomaton.hpp"
#include "Brzozowski_derivative.cpp"

std::string BrzozowskiAutomaton::findFirstUncompleted() {
    for (auto elem: completeStates) {
        if (!elem.second) return elem.first;
    }
    return "";
}

void BrzozowskiAutomaton::addDerivativeBySymbol(Node<std::pair<int, std::string >> *t, char c,
                                                int& curState, int oldState) {
    auto newTree = derivative(t, c);
    while (hasEmpty(newTree)) {
        newTree = ACI(newTree);
    }
    std::string newRegex = sortRegex(newTree);
    if (!this->map.contains(newRegex)) {
        map[newRegex] = curState;
        transitions.push_back(std::make_pair(std::make_pair(oldState, c), curState));
        completeStates[newRegex] = false;
        if (containsEPS(newRegex)) finalStates.push_back(curState);
        curState++;
    } else {
        int toState = map[newRegex];
        transitions.push_back(std::make_pair(std::make_pair(oldState, c), toState));
    }
}

void BrzozowskiAutomaton::addDerivativeByAlphabet(std::string regex,
                                                  int &curState) {
    auto tree = buildTree(infixToPostfix(get_lexems(regex)));
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
    if (containsEPS(initialRegex)) finalStates.push_back(0);
    completeStates[initialRegex] = false;
    int curState = 1;
    while (!findFirstUncompleted().empty()) {
        auto regex = findFirstUncompleted();
        addDerivativeByAlphabet(regex, curState);
    }
}
