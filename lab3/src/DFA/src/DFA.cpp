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

void invert_dfs(int start, std::set<int> &visited, std::unordered_map<int, std::set<int>> &back_trans) {
    visited.insert(start);
    for (auto state: back_trans[start]) {
        if (!visited.contains(state))
            invert_dfs(state, visited, back_trans);
    }
}

void dfs(int start, std::set<int> &visited, std::unordered_map<int, std::set<int>> &trans) {
    visited.insert(start);
    for (auto state: trans[start]) {
        if (!visited.contains(state))
            dfs(state, visited, trans);
    }
}

void DFA::deleteTrap() {
    this->renumeration();
    std::set<int> visited1, visited2;
    std::unordered_map<int, std::set<int>> back_trans;
    std::unordered_map<int, std::set<int>> tmp_trans;

    for (auto trans: transitions) {
        back_trans[trans.second].insert(trans.first.first);
        tmp_trans[trans.first.first].insert(trans.second);
    }

    dfs(0, visited2, tmp_trans);

    for (auto final: final_states) {
        if (!visited1.contains(final))
            invert_dfs(final, visited1, back_trans);
    }

    std::set<int> old_states;
    std::set<int> to_delete;
    for (int i = 0; i < count_states; i++)
        old_states.insert(i);

    std::set<int> visited;
    std::set_intersection(visited1.begin(), visited1.end(), visited2.begin(), visited2.end(),
                          std::inserter(visited, visited.begin()));

    std::set_difference(old_states.begin(), old_states.end(), visited.begin(), visited.end(),
                        std::inserter(to_delete, to_delete.begin()));

    count_states -= to_delete.size();

    std::erase_if(transitions, [&](std::pair<std::pair<int, char>, int> x) -> bool {
        return to_delete.contains(x.first.first) || to_delete.contains(x.second);
    });
    std::erase_if(final_states, [&](int x) -> bool {
        return to_delete.contains(x);
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
        if (transitions_map[cur_state].empty() || final_states.contains(cur_state) && std::rand() % 100 >= 70)
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
    dfa.buildTransitionsMap();
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
    }
    dfa.buildTransitionsMap();
    return dfa;
}

DFA DFA::invert() {
    DFA dfa = returnTrap(*this);
    DFA invertedDFA(dfa.getAlphabet());
    std::set<int> states;
    for (auto state: dfa.transitions_map) {
        states.insert(state.first);
        for (auto to: state.second) {
            invertedDFA.addTransition(state.first, to.first, to.second);
            states.insert(to.second);
        }
        if (!final_states.contains(state.first)) {
            invertedDFA.final_states.insert(state.first);
        }
    }
    invertedDFA.setCount(states.size());
    invertedDFA.deleteTrap();
    invertedDFA.renumeration();

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
    dfa1.renumeration();
    dfa2.renumeration();
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
                    if (dfa1.final_states.contains(item.second) &&
                        dfa2.final_states.contains(state2[item.first]) ||
                        !dfa1.final_states.contains(item.second) &&
                        !dfa2.final_states.contains(state2[item.first])) {
                        intersection_map[{i, j}][item.first] = {item.second, state2[item.first]};
                    }
                }
            }
            for (auto item: state2) {
                if (state1.contains(item.first)) {
                    if (dfa2.final_states.contains(item.second) &&
                        dfa1.final_states.contains(state1[item.first]) ||
                        !dfa2.final_states.contains(item.second) &&
                        !dfa1.final_states.contains(state1[item.first])) {
                        intersection_map[{i, j}][item.first] = {state1[item.first], item.second};
                    }
                }
            }
        }
    }

    std::set<int> states;
    auto map = mapping(dfa1.getSize(), dfa2.getSize());
    for (const auto &item: intersection_map) {
        for (auto by: item.second) {
            states.insert(map[item.first.first][item.first.second]);
            states.insert(map[by.second.first][by.second.second]);
            product.addTransition(map[item.first.first][item.first.second],
                                  by.first, map[by.second.first][by.second.second]);
        }
    }
    auto final_states1 = dfa1.getFinalStates();
    auto final_states2 = dfa2.getFinalStates();
    for (int states1: final_states1) {
        for (int states2: final_states2) {
            product.addFinalState(map[states1][states2]);
            states.insert(map[states1][states2]);
        }
    }
    product.setCount(states.size());
    product.buildTransitionsMap();
    return product;
}

void DFA::renumeration() {
    std::set<int> states;
    for (auto &trans: transitions) {
        states.insert(trans.first.first);
        states.insert(trans.second);
    }

    std::unordered_map<int, int> renum;
    int cur_state = 0;
    for (auto state: states) {
        renum[state] = cur_state++;
    }

    for (auto &trans: transitions) {
        trans.first.first = renum[trans.first.first];
        trans.second = renum[trans.second];
    }

    std::set<int> tmp_final(final_states);
    final_states.clear();
    for (auto &final: tmp_final) {
        final_states.insert(renum[final]);
    }

    std::unordered_map<int, std::unordered_map<char, int>> tmp_trans(transitions_map);
    transitions_map.clear();

    for (auto &trans: tmp_trans) {
        std::unordered_map<char, int> mp;
        for (auto &to: trans.second) {
            mp[to.first] = mp[renum[to.second]];
        }
        transitions_map[renum[trans.first]] = mp;
    }
}