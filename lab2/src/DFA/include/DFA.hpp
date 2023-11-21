#ifndef LAB2_DFA_HPP
#define LAB2_DFA_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        std::hash<int> int_hasher;
        return int_hasher(v.first) ^ int_hasher(v.second);
    }
};

class DFA {
private:
    std::vector<std::vector<bool>> reachabilityMatrix;
    std::unordered_map<std::pair<int, int>, std::vector<char>, pair_hash> transitions;
    std::unordered_set<int> finalStates;
    std::vector<int> getRandomTraversal();
public:
    explicit DFA(std::string);
    std::string getRandomString();
};

#endif //LAB2_DFA_HPP
