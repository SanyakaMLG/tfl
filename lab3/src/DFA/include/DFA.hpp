#ifndef LAB3_DFA_HPP
#define LAB3_DFA_HPP
#include <vector>
#include <set>
#include <unordered_map>
#include <string>

class DFA {
private:
    int count_states{};

    std::set<char> alphabet;
public:
    std::set<int> final_states;
    std::vector<std::pair<std::pair<int, char>, int>> transitions;
    std::unordered_map<int, std::unordered_map<char, int>> transitions_map;
    DFA(std::set<char> alphabet): alphabet(alphabet) {}
    bool checkString(std::string&);
    void addTransition(int from, char c, int to);
    void addFinalState(int state);
    void setCount(int count);
    void deleteTrap();
    DFA invert();
    void printDot();
    int getSize();
    void clear();
    void buildTransitionsMap();
    std::set<char> getAlphabet();
    std::string getRandomString();
    std::set<int> getFinalStates();
    void renumeration();
};
DFA intersect(DFA &dfa1, DFA &DFA2);
#endif //LAB3_DFA_HPP
