#include "DFA.hpp"
#include <iostream>
#include <random>
#include <map>

#define TRAP 1000

void DFA::addFinalState(int state) {
    final_states.insert(state);
}

void DFA::setCount(int count) {
    count_states = count;
}

void DFA::addTransition(int from, char c, int to) {
    transitions.emplace_back(std::make_pair(from, c), to);
    transitions_map[from][c] = to;
}

void dfs(int start, std::set<int> &visited, std::unordered_map<int, std::set<int>> &back_trans) {
    visited.insert(start);
    for (auto state: back_trans[start]) {
        if (!visited.contains(state))
            dfs(state, visited, back_trans);
    }
}

void DFA::deleteTrap() {
    std::set<int> visited;
    std::unordered_map<int, std::set<int>> back_trans;

    for (auto trans: transitions) {
        back_trans[trans.second].insert(trans.first.first);
    }

    for (auto final: final_states) {
        if (!visited.contains(final))
            dfs(final, visited, back_trans);
    }

    std::set<int> old_states;
    std::set<int> to_delete;
    for (int i = 0; i < count_states; i++)
        old_states.insert(i);

    std::set_difference(old_states.begin(), old_states.end(), visited.begin(), visited.end(),
                        std::inserter(to_delete, to_delete.begin()));

    count_states -= to_delete.size();

    std::erase_if(transitions, [&](std::pair<std::pair<int, char>, int> x) -> bool {
        return to_delete.contains(x.first.first) || to_delete.contains(x.second);
    });

    transitions_map.clear();
    for (auto trans: transitions) {
        transitions_map[trans.first.first][trans.first.second] = trans.second;
    }
}

void DFA::printDot() {
    std::string res = "digraph G {\nrankdir=\"LR\";\n";
    for (auto trans: transitions) {
        res.append(std::to_string(trans.first.first) + " -> " +
                   std::to_string(trans.second) + " [label=\"" + trans.first.second + "\"];\n");
    }

    for (auto i: final_states) {
        std::string shape = final_states.contains(i) ? "doublecircle" : "circle";
        res.append(std::to_string(i) + " [shape=\"" + shape + "\"];\n");
    }
    res.append("}");
    std::cout << res << std::endl;
}

int DFA::getSize() {
    return this->count_states;
}

void DFA::clear() {
    count_states = 0;
    transitions.clear();
    transitions_map.clear();
    final_states.clear();
    alphabet.clear();
}

bool DFA::checkString(std::string &str) {
    int cur_state = 0;
    for (auto c: str) {
        if (!transitions_map[cur_state].contains(c))
            return false;

        cur_state = transitions_map[cur_state][c];
    }

    return final_states.contains(cur_state);
}

void DFA::buildTransitionsMap() {
    transitions_map.clear();
    for (auto &trans: transitions) {
        transitions_map[trans.first.first][trans.first.second] = trans.second;
    }
}

template<typename KeyType, typename ValueType>
std::pair<KeyType, ValueType> randomChooseFromMap(const std::unordered_map<KeyType, ValueType> &myMap) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, myMap.size() - 1);
    int randomIndex = dis(gen);
    auto it = myMap.begin();
    std::advance(it, randomIndex);
    return *it;
}

std::string DFA::getRandomString() {
    int cur_state = 0;
    std::string res;
    while (true) {
        if (transitions_map[cur_state].empty() || final_states.contains(cur_state) && std::rand() % 100 >= 80)
            break;

        auto pair = randomChooseFromMap(transitions_map[cur_state]);
        res.append(std::string(1, pair.first));
        cur_state = pair.second;
    }
    return res;
}

std::set<char> DFA::getAlphabet() {
    return alphabet;
}

std::set<int> DFA::getFinalStates() {
    return final_states;
}

DFA returnTrap(DFA normDFA) {
    DFA dfa = normDFA;
    bool flag = false;
    auto alphabet = dfa.getAlphabet();
    for (auto state: dfa.transitions_map) {
        for (char letter: alphabet) {
            if (!state.second.contains(letter)) {
                flag = true;
                dfa.addTransition(state.first, letter, TRAP);
            }
        }
    }
    if (flag) {
        for (char letter: alphabet) {
            dfa.addTransition(TRAP, letter, TRAP);
        }
        dfa.addFinalState(TRAP);
        dfa.setCount(dfa.getSize()+1);
    }
    dfa.buildTransitionsMap();
    return dfa;
}

DFA DFA::invert() {
    DFA dfa = returnTrap(*this);
    DFA invertedDFA(dfa.getAlphabet());
    invertedDFA.setCount(dfa.getSize());

    for (auto state: dfa.transitions_map) {
        for (auto to: state.second) {
            invertedDFA.addTransition(state.first, to.first, to.second);
        }
        std::cout<<state.first<<'\n';
        if (!this->final_states.contains(state.first)) {
            invertedDFA.addFinalState(state.first);
        }
    }
    invertedDFA.printDot();
    invertedDFA.deleteTrap();
    return invertedDFA;
}

static std::vector<std::vector<int>> mapping(int size1, int size2) {
    std::vector<std::vector<int>> ans(size1, std::vector<int>(size2, 0));
    int k = 0;
    for (int i = 0; i < size1; ++i) {
        for (int j = 0; j < size2; ++j) {
            ans[i][j] = k++;
        }
    }
    return ans;
}


DFA intersect(DFA &dfa1, DFA &dfa2) {
    std::set<char> intersection;
    auto alphabet1 = dfa1.getAlphabet();
    auto alphabet2 = dfa2.getAlphabet();
    std::set_intersection(alphabet1.begin(),
                          alphabet1.end(),
                          alphabet2.begin(),
                          alphabet2.end(),
                          std::inserter(intersection, intersection.begin()));
    DFA product = DFA(intersection);
    std::map<std::pair<int, int>, std::map<char, std::pair<int, int>>> intersection_map;
    dfa1.buildTransitionsMap();
    dfa2.buildTransitionsMap();
    for (int i = 0; i < dfa1.getSize(); ++i) {
        for (int j = 0; j < dfa2.getSize(); ++j) {
            auto state1 = dfa1.transitions_map[i];
            auto state2 = dfa2.transitions_map[j];
            for (auto item: state1) {
                if (state2.contains(item.first)) {
                    intersection_map[{i, j}][item.first] = {item.second, state2[item.first]};
                }
            }
            for (auto item: state2) {
                if (state1.contains(item.first)) {
                    intersection_map[{i, j}][item.first] = {state1[item.first], item.second};
                }
            }
        }
    }
    auto map = mapping(dfa1.getSize(), dfa2.getSize());
    for (const auto &item: intersection_map) {
        for (auto by: item.second) {
            product.addTransition(map[item.first.first][item.first.second],
                                  by.first, map[by.second.first][by.second.second]);
        }
    }
    auto final_states1 = dfa1.getFinalStates();
    auto final_states2 = dfa2.getFinalStates();
    for (int states1: final_states1) {
        for (int states2: final_states2) {
            product.addFinalState(map[states1][states2]);
        }
    }
    product.buildTransitionsMap();
    return product;
}