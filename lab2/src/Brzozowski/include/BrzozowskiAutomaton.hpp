#ifndef LAB2_BRZOZOWSKIAUTOMATON_HPP
#define LAB2_BRZOZOWSKIAUTOMATON_HPP

#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include "Node.hpp"



class BrzozowskiAutomaton {
public:
    std::unordered_map<std::string, int> map; // mapping regex with automation state
    std::vector<std::pair<std::pair<int, std::string>, int>>  transitions;
    int initialState{};
    std::string initialRegex;
    std::vector<int> finalStates;
    std::unordered_map<std::string, bool> completeStates;
    std::unordered_set<char> alphabet;
    std::string findFirstUncompleted();
    void addDerivativeBySymbol(Node *, char, int&, int);
    void addDerivativeByAlphabet(std::string, int&);
    void addTransition(int stateFrom, int stateTo, char c);
    void addTransition(std::pair<std::pair<int, std::string>, int>);
    int refactorStates();
    void deleteState(int);
    void getTransitionFromToN(
            int, std::vector<std::pair<std::pair<int, std::string>, int>>&,
            std::vector<std::pair<std::pair<int, std::string>, int>>&
            );
public:
    explicit BrzozowskiAutomaton(std::string str);
    std::string convertToRegex();
    std::string getDot();
};

#endif //LAB2_BRZOZOWSKIAUTOMATON_HPP