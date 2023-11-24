#ifndef LAB3_DFA_HPP
#define LAB3_DFA_HPP
#include <vector>
#include <set>
#include <unordered_map>
#include <string>

class DFA {
private:
    int count_states;
    std::vector<std::pair<std::pair<int, char>, int>> transitions;
    std::set<int> final_states;
    std::set<char> alphabet;
public:
    DFA(std::set<char> alphabet): alphabet(alphabet) {}
    void addTransition(int from, char c, int to);
    void addFinalState(int state);
    void setCount(int count);
    void deleteTrap();
    void printDot();
};

#endif //LAB3_DFA_HPP