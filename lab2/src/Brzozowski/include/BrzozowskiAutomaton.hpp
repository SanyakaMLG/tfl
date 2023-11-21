#ifndef LAB2_BRZOZOWSKIAUTOMATON_HPP
#define LAB2_BRZOZOWSKIAUTOMATON_HPP

#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include "Node.hpp"



class BrzozowskiAutomaton {
private:
    std::unordered_map<std::string, int> map; // mapping regex with automation state
    std::vector<std::pair<std::pair<int, std::string>, int>>  transitions;
    std::string initialRegex;
    std::unordered_set<int> finalStates;
    std::unordered_map<std::string, bool> completeStates;
    std::unordered_set<char> alphabet;
    int countStates;
    std::string findFirstUncompleted();
    void addDerivativeBySymbol(Node *, char, int&, int, bool);
    void addDerivativeByAlphabet(std::string, int&, bool);
    void addTransition(int stateFrom, int stateTo, char c);
    void addTransition(std::pair<std::pair<int, std::string>, int>);
    int refactorStates();
    void deleteState(int);
    void getTransitionFromToN(
            int, std::vector<std::pair<std::pair<int, std::string>, int>>&,
            std::vector<std::pair<std::pair<int, std::string>, int>>&
            );
public:
    explicit BrzozowskiAutomaton(std::string, bool=false);
    std::string convertToRegex();
    std::string getDot();
    std::vector<std::pair<std::pair<int, std::string>, int>> getTransitions();
    int getCountStates();
    std::unordered_set<int> getFinalStates();
};

#endif //LAB2_BRZOZOWSKIAUTOMATON_HPP