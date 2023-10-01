//
// Created by aleks on 30.09.2023.
//

#ifndef LAB2_BRZOZOWSKIAUTOMATON_HPP
#define LAB2_BRZOZOWSKIAUTOMATON_HPP

#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include "BinaryTree.hpp"

#endif //LAB2_BRZOZOWSKIAUTOMATON_HPP

class BrzozowskiAutomaton {
private:
    std::unordered_map<std::string, int> map; // mapping regex with automation state
    std::vector<std::pair<std::pair<int, char>, int>>  transitions;
    int initialState{};
    std::string initialRegex;
    std::vector<int> finalStates;
    std::unordered_map<std::string, bool> completeStates;
    std::unordered_set<char> alphabet;
    bool containsEPS(std::string);
    std::string findFirstUncompleted();
    void addDerivativeBySymbol(Node<std::pair<int, std::string >> *, char, int&, int);
    void addDerivativeByAlphabet(std::string, int&);
public:
    explicit BrzozowskiAutomaton(std::string str):initialRegex(std::move(str)){};
    void addTransition(std::string regexFrom, std::string regexTo, char c);
    std::string convertToRegex();
};
