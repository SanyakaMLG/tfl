#include <iostream>
#include <set>
#include <vector>
#include <map>

namespace Eearley {
    using grammar_t = std::map<std::string, std::set<std::vector<std::string>>>;
    using earley_grammar_t = std::map<std::string, std::set<std::string>>;
    using state_t = std::tuple<std::string, std::vector<std::string>,size_t, int>;
    using set_of_states_t = std::set<state_t>;
    using situation_map_t = std::map<std::string, set_of_states_t>;
    using s_vector = std::vector<std::string>;
}
using namespace Eearley;

void addZeroState(grammar_t &grammar) {
    if (grammar.contains("S")) {
        std::vector<std::string> v(1, "S");
        grammar["[ZERO]"].insert(v);
    }
    if (grammar.contains("S^")) {
        std::vector<std::string> v(1, "S^");
        grammar["[ZERO]"].insert(v);
    }
}

std::vector<situation_map_t> init(size_t size, grammar_t &grammar) {
    situation_map_t D0;
    std::vector<situation_map_t> states;
    if (grammar.contains("S^"))
        D0["S^"].insert(state_t("[ZERO]", s_vector(1,"S^"),0, 0));
    else
        D0["S"].insert(state_t("[ZERO]",  s_vector(1,"S"),0, 0));
    states.push_back(D0);
    for (int i = 1; i < size; i++) {
        situation_map_t D;
        states.push_back(D);
    }
    return states;
}

std::pair<state_t, std::string> createStateScan(state_t &old_state) {
    auto left = std::get<0>(old_state);
    auto right = std::get<1>(old_state);
    std::string next;
    size_t pos = std::get<2>(old_state);
    if (pos+1>= right.size()) {
        next = "$";
        pos++;
    } else {
        pos++;
        next = right[pos];
    }
    state_t state = std::make_tuple(left, right, pos, std::get<3>(old_state));
    return {state, next};
}

void scan(std::vector<situation_map_t> &situations, size_t j, std::string &word) {
    if (j == 0)
        return;
    if (situations[j - 1].contains(std::string(1, word[j - 1]))) {
        for (auto item: situations[j - 1][std::string(1, word[j - 1])]) {
            auto state = createStateScan(item);
            situations[j][state.second].insert(state.first);
        }
    }
}

std::pair<state_t, std::string> createStateComplete(state_t &old_state, size_t j) {
    auto left = std::get<0>(old_state);
    auto right = std::get<1>(old_state);
    std::string next;
    size_t pos = std::get<2>(old_state);
    if (pos+1>= right.size()) {
        pos++;
        next = "$";
    } else {
        pos++;
        next = right[pos];
    }
    state_t state = std::make_tuple(left, right,pos, j);
    return {state, next};

}

bool complete(std::vector<situation_map_t> &situations, size_t j) {
    bool changed = false;
    if (situations[j].contains("$")) {
        for (auto child: situations[j]["$"]) {
            size_t idx = std::get<3>(child);
            std::string left = std::get<0>(child);
            if (situations[idx].contains(left)) {
                for (auto parent: situations[idx][left]) {
                    auto state = createStateComplete(parent, std::get<3>(parent));
                    size_t size = situations[j][state.second].size();
                    situations[j][state.second].insert(state.first);
                    if (size - situations[j][state.second].size() != 0) {
                        changed = true;
                    }
                }
            }
        }
    }
    return changed;
}

bool createStatePredict(situation_map_t &situation, const std::string &left,const s_vector &rule,
                        size_t j) {
    bool changed = false;
    std::string key = rule[0];
    state_t state = std::make_tuple(left, rule, 0, j);
    size_t size = situation[key].size();
    situation[key].insert(state);
    if (size - situation[key].size() != 0) {
        changed = true;
    }
    return changed;
}

bool predict(std::vector<situation_map_t> &situations, size_t j, grammar_t &grammar) {
    bool changed = false;
    for (auto &rules: situations[j]) {
        if (grammar.contains(rules.first)) {
            for (auto &rule: grammar[rules.first]) {
                changed = createStatePredict(situations[j], rules.first, rule, j) || changed;
            }
        }
    }
    return changed;
}

earley_grammar_t modifyGrammar(grammar_t &grammar) {
    earley_grammar_t newGrammar;
    for (const auto &rule: grammar) {

        for (const auto &item: rule.second) {
            std::string str;
            for (const auto &letter: item) {
                str.append(letter);
            }
            newGrammar[rule.first].insert(str);
        }
    }
    return newGrammar;
}

bool Earley(grammar_t &grammar, std::string &word) {
    if(word.empty() && grammar.contains("S^")){
        return true;
    }else{
        if (grammar.contains("S^")){
            grammar["S^"].erase(std::vector<std::string>(1,"eps"));
        }
    }
    addZeroState(grammar);
    //auto new_grammar = modifyGrammar(grammar);
    auto states = init(word.length()+1, grammar);
    for (size_t j = 0; j <= word.length(); j++) {
        scan(states, j, word);
        int flag1=1;
        int flag2=1;
        while (flag1 != 0 or flag2 !=0) {
            flag1 = 0;
            flag2 = 0;
            bool changed1 = true;
            while (changed1) {
                changed1 = complete(states, j);
                if(changed1)
                    flag1--;
            }
            bool changed2 = true;
            while (changed2) {
                changed2 = predict(states, j, grammar);
                if(changed2)
                    flag2--;
            }
        }
    }
    bool ans = states[word.length()]["$"].contains(std::make_tuple("[ZERO]",s_vector (1,"S"),1, 0));
    ans = ans || states[word.length()]["$"].contains(std::make_tuple("[ZERO]",s_vector (1,"S^"),1, 0));
    return ans;
}