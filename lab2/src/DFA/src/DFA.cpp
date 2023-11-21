#ifndef LAB2_DFA_CPP
#define LAB2_DFA_CPP

#include "DFA.hpp"
#include "BrzozowskiAutomaton.hpp"

template <typename T>
T chooseRandom(std::vector<T> vector) {
    return vector[std::rand() % vector.size()];
}

DFA::DFA(std::string initialRegex) {
    std::srand(time(0));
    BrzozowskiAutomaton automata = BrzozowskiAutomaton(initialRegex, true);
    this->finalStates = automata.getFinalStates();
    int n = automata.getCountStates();
    reachabilityMatrix = std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));
    auto vectorTransitions = automata.getTransitions();
    for (auto trans: vectorTransitions) {
        auto pair = std::make_pair(trans.first.first, trans.second);
        n = std::max(std::max(pair.first, pair.second), n);
        transitions[pair].push_back(trans.first.second[0]);
        reachabilityMatrix[pair.first][pair.second] = true;
    }
}

std::vector<int> getTrueIndexes(std::vector<bool> vector) {
    std::vector<int> result;
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i]) result.push_back(i);
    }
    return result;
}

std::vector<int> DFA::getRandomTraversal() {
    int curState = 0;
    std::vector<int> traversal;
    traversal.push_back(curState);
    while (true) {
        auto possibleWays = getTrueIndexes(reachabilityMatrix[curState]);
        if (possibleWays.size() == 0)
            return traversal;
        if (finalStates.contains(curState) && std::rand() % 10 == 9) break;
        curState = chooseRandom(possibleWays);
        traversal.push_back(curState);
    }
    return traversal;
}

std::string DFA::getRandomString() {
    std::vector<int> traversal = getRandomTraversal();
    std::string result = "";
    for (int i = 1; i < traversal.size(); i++) {
        auto pair = std::make_pair(traversal[i - 1], traversal[i]);
        result.append(std::string(1, chooseRandom(transitions[pair])));
    }
    return result;
}

#endif //LAB2_DFA_CPP