#include "DFA.hpp"
#include <iostream>

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
}

void DFA::printDot() {
    std::string res = "digraph G {\nrankdir=\"LR\";\n";
    for (auto trans: transitions) {
        res.append(std::to_string(trans.first.first) + " -> " +
                        std::to_string(trans.second) + " [label=\"" + trans.first.second + "\"];\n");
    }

    for (int i = 0; i < count_states; i++) {
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
