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

std::string BrzozowskiAutomaton::getDot() {
    std::string s = "digraph G {\nrankdir=\"LR\";\n";
    for (auto el: transitions) {
        s.append(std::to_string(el.first.first) + " -> " + std::to_string(el.second) + " [label=\"" + el.first.second + "\"];\n");
    }
    for (auto el: map) {
        std::string shape =
                finalStates.contains(el.second) ? "doublecircle" : "circle";
        s.append(std::to_string(el.second) + " [label=\"" + el.first + "\", shape=" + shape + "];\n");
    }
    s.append("}");
    return s;
}

void BrzozowskiAutomaton::addTransition(int stateFrom, int stateTo, char c) {
    for (auto &el: transitions) {
        if (el.first.first == stateFrom && el.second == stateTo) {
            std::string firstArgument = el.first.second == "" ? "(eps)" : el.first.second;
            el.first.second = "(" + firstArgument + "|" + std::string(1, c) + ")";
            return;
        }
    }
    transitions.emplace_back(std::make_pair(stateFrom, std::string(1, c)), stateTo);
}

void BrzozowskiAutomaton::addTransition(std::pair<std::pair<int, std::string>, int> trans) {
    for (auto &el: transitions) {
        if (el.first.first == trans.first.first && el.second == trans.second) {
            std::string firstArgument = el.first.second == "" ? "(eps)" : el.first.second;
            el.first.second = "(" + firstArgument + "|" + trans.first.second + ")";
            return;
        }
    }
    transitions.push_back(trans);
}

std::string BrzozowskiAutomaton::findFirstUncompleted() {
    for (auto elem: completeStates) {
        if (!elem.second) return elem.first;
    }
    return "";
}

void BrzozowskiAutomaton::addDerivativeBySymbol(Node *t, char c,
                                                int& curState, int oldState, bool fuzzing) {
    Regex regex(t);
    auto newTree = regex.der( c);
    std::string newRegex = newTree.get();
    if (!this->map.contains(newRegex)) {
        map[newRegex] = curState;
        transitions.emplace_back(std::make_pair(oldState, std::string(1, c)), curState);
        completeStates[newRegex] = false;
        if (containsEPS(newTree.getTree())) finalStates.insert(curState);
        curState++;
        countStates++;
    } else {
        int toState = map[newRegex];
        if (!fuzzing)
            addTransition(oldState, toState, c);
        else
            transitions.emplace_back(std::make_pair(oldState, std::string(1, c)), toState);
    }
}

void BrzozowskiAutomaton::addDerivativeByAlphabet(std::string regex,
                                                  int &curState,
                                                  bool fuzzing) {
    auto tree = Regex(regex).getTree();
    for (auto c: this->alphabet) {
        addDerivativeBySymbol(tree, c, curState, map[regex], fuzzing);
    }
    completeStates[regex] = true;
}

BrzozowskiAutomaton::BrzozowskiAutomaton(std::string initialRegex, bool fuzzing) {
    for (auto c: initialRegex) {
        if (std::isalpha(c)) this->alphabet.insert(c);
    }
    Regex initialRegex1 = Regex(initialRegex);
    map[initialRegex1.get()] = 0;
    if (containsEPS(initialRegex1.getTree())) finalStates.insert(0);
    completeStates[initialRegex1.get()] = false;
    int curState = 1;
    countStates = 1;
    while (!findFirstUncompleted().empty()) {
        auto regex = findFirstUncompleted();
        addDerivativeByAlphabet(regex, curState, fuzzing);
    }
    if (fuzzing && map.contains("∅")) {
        std::erase_if(transitions, [&](std::pair<std::pair<int, std::string>, int> x) -> bool {
            return x.first.first == map["∅"] || x.second == map["∅"];
        });
        map.erase("∅");
    }
}

std::vector<std::pair<std::pair<int, std::string>, int>> BrzozowskiAutomaton::getTransitions() {
    return this->transitions;
}

int BrzozowskiAutomaton::getCountStates() {
    return this->countStates;
}

std::unordered_set<int> BrzozowskiAutomaton::getFinalStates() {
    return this->finalStates;
}

int BrzozowskiAutomaton::refactorStates() {
    int newFinalState = map.size();
    int start = 1;
    if (map.contains("∅")) {
        int toDelete = map["∅"];
        std::erase_if(transitions, [&](std::pair<std::pair<int, std::string>, int> x) -> bool
        {return x.first.first == toDelete || x.second == toDelete;});
        map.erase("∅");
    }
    for (auto finalState: finalStates) {
        transitions.emplace_back(std::make_pair(finalState, ""), newFinalState);
    }
    finalStates.clear();
    for (auto trans: transitions) {
        if (trans.second == 0) {
            transitions.emplace_back(std::make_pair(-1, ""), 0);
            start = 0;
            break;
        }
    }
    return start;
}

void BrzozowskiAutomaton::getTransitionFromToN(
        int n, std::vector<std::pair<std::pair<int, std::string>, int>> &from,
        std::vector<std::pair<std::pair<int, std::string>, int>> &to) {

    for (auto transition: transitions) {
        if (transition.first.first == n) from.push_back(transition);
        if (transition.second == n) to.push_back(transition);
    }
}

bool isTrapState(std::vector<std::pair<std::pair<int, std::string>, int>> &from) {
    if (from.size() == 0 || from.size() == 1 && from[0].first.first == from[0].second) return true;
    return false;
}

std::string findCycle(
        std::vector<std::pair<std::pair<int, std::string>, int>> &from,
        std::vector<std::pair<std::pair<int, std::string>, int>> &to
        ) {
    std::string res = "";
    for (auto trans: from) {
        if (trans.second == trans.first.first) {
            std::string underStar = "";
            if (trans.first.second.size() > 1)
                underStar = "(" + trans.first.second + ")";
            else
                underStar = trans.first.second;
            res = underStar + "*";
            std::erase(to, trans);
            std::erase(from, trans);
            break;
        }
    }
    return res;
}

std::vector<std::pair<std::pair<int, std::string>, int>> mergeTransitions(
        std::vector<std::pair<std::pair<int, std::string>, int>> &from,
        std::vector<std::pair<std::pair<int, std::string>, int>> &to
        ) {
    auto cycle = findCycle(from, to);
    std::vector<std::pair<std::pair<int, std::string>, int>> res;
    for (auto transFrom: from) {
        for (auto transTo: to) {
            res.emplace_back(std::make_pair(
                    transTo.first.first,
                    transTo.first.second + cycle + transFrom.first.second), transFrom.second);
        }
    }
    return res;
}

void BrzozowskiAutomaton::deleteState(int n) {
    std::vector<std::pair<std::pair<int, std::string>, int>> from, to;
    getTransitionFromToN(n, from, to);
    std::vector<std::pair<std::pair<int, std::string>, int>> newTransitions;
    if (!isTrapState(to)) {
        newTransitions = mergeTransitions(from, to);
    }
    erase_if(transitions, [&](std::pair<std::pair<int, std::string>, int> x) -> bool
                                        {return x.first.first == n || x.second == n;});
    for (auto newTrans: newTransitions) {
        addTransition(newTrans);
    }
//    transitions.insert(transitions.end(), newTransitions.begin(), newTransitions.end());
}

std::string BrzozowskiAutomaton::convertToRegex() {
    BrzozowskiAutomaton copyAutomaton = BrzozowskiAutomaton{*this};
    int i = copyAutomaton.refactorStates();
    //std::cout << copyAutomaton.getDot() << "\n\n\n";
    for (i; i < map.size(); i++) {
        copyAutomaton.deleteState(i);
//        std::cout << copyAutomaton.getDot() << "\n\n\n";
    } // ε
    std::string regex = "";
    for (auto trans: copyAutomaton.transitions) {
        if (trans.first.second == "") regex.append("(eps)|");
        else regex.append(trans.first.second + "|");
    }
    return regex.substr(0, regex.size() - 1);
}

#endif