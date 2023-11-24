#include "utils.hpp"


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>

/*
 Правила записи грамматики:
 Нетерминалы - заглавные буквы
 Терминалы - строчные или другие символы(за исключением специальных)*/



void optimizeGrammar(Grammar &grammar){
    for (auto& item: grammar.grammar){
        for(auto it =item.second.begin(); it!=item.second.end();){
            if(std::vector<std::string>(1, item.first) ==*it){
                it = item.second.erase(it);
            } else{
                it++;
            }
        }
    }
}


void getRulePrefix(Grammar &grammar,
                   const std::string &left,
                   const std::set<std::vector<std::string>> &right) {
    std::string prefix_left = left + "^";
    for (const auto &rule: right) {
        std::vector<std::string> prefix;
        for (std::string letter: rule) {
            std::vector<std::string> new_rule;
            prefix.push_back(letter);
            if (isupper(letter[0])) {
                std::vector<std::string> copy = prefix;
                copy.back() = copy.back().append("^");
                new_rule.insert(new_rule.end(), copy.begin(), copy.end());
            } else {
                new_rule.insert(new_rule.end(), prefix.begin(), prefix.end());
            }
            grammar.grammar[prefix_left].insert(new_rule);
        }
    }
}

Grammar getPrefixLanguageGrammar(Grammar &grammar) {
    Grammar prefix_grammar;
    for (const auto &rule: grammar.grammar) {
        prefix_grammar.grammar[rule.first] = rule.second;
    }
    prefix_grammar.ZeroState += grammar.ZeroState+"^";
    prefix_grammar.grammar[prefix_grammar.ZeroState].insert(std::vector<std::string>(1, "e"));
    for (const auto &rule: grammar.grammar) {
        getRulePrefix(prefix_grammar, rule.first, rule.second);
    }
    optimizeGrammar(prefix_grammar);
    return prefix_grammar;
}

void printGrammar(Grammar &grammar) {
    for (const auto &rule: grammar.grammar) {
        std::cout << rule.first << " -> ";
        size_t i = rule.second.size();
        for (const auto &r: rule.second) {
            i--;
            for (const auto &elem: r) {
                std::cout << elem;
            }
            if (i != 0)
                std::cout << '|';
        }
        std::cout << std::endl;
    }
}

void reverseRule(Grammar &grammar, const std::string &left,
                 std::set<std::vector<std::string>> &right) {
    for (auto &item: right) {
        std::vector<std::string> reverse(item.size());
        size_t i = item.size() - 1;
        for (auto &let: item) {
            reverse[i] = let;
            i--;
        }
        grammar.grammar[left].insert(reverse);
    }
}

Grammar reverseGrammar(Grammar &grammar) {
    Grammar reverse_grammar;
    reverse_grammar.ZeroState = grammar.ZeroState;
    for (auto &rule: grammar.grammar) {
        reverseRule(reverse_grammar, rule.first, rule.second);
    }
    return reverse_grammar;
}


std::set<std::string> getEpsRules(Grammar &grammar) {
    std::set<std::string> ans;
    for (const auto &rules: grammar.grammar) {
        if (rules.second.contains(std::vector<std::string>(1, "e"))) {
            ans.insert(rules.first);
            grammar.grammar[rules.first].erase(std::vector<std::string>(1, "e"));
        }
    }
    bool changed = true;
    while (changed) {
        size_t len = ans.size();
        for (const auto &rules: grammar.grammar) {
            for (const auto &v_rules: rules.second) {
                bool contains = true;
                for (const auto &letter: v_rules) {
                    if (!ans.contains(letter)) {
                        contains = false;
                    }
                }
                if (contains) {
                    ans.insert(rules.first);
                }
            }
        }
        if (len == ans.size()) {
            changed = false;
        }
    }
    return ans;
}

std::vector<int> getIndex(std::string &let, std::vector<std::string> &rules) {
    std::vector<int> ans;
    for (int i = 0; i < rules.size(); i++) {
        if (let == rules[i]) {
            ans.push_back(i);
        }
    }
    return ans;
}

void getSubsets(std::vector<int> &idxes, std::vector<std::vector<int>> &subsets, std::vector<int> &subset, int idx) {
    subsets.push_back(subset);
    for (int i = idx; i < idxes.size(); i++) {
        subset.push_back(idxes[i]);
        getSubsets(idxes, subsets, subset, i + 1);
        subset.pop_back();
    }
}

std::vector<std::string> getNewRule(std::vector<int> &idx, std::vector<std::string> &rule) {
    std::vector<std::string> newRule = rule;
    for (int i: idx) {
        newRule.erase(newRule.begin() + i);
    }
    return newRule;
}

Grammar deleteEpsRules(Grammar &grammar) {
    Grammar new_grammar;
    for (auto rule: grammar.grammar) {
        new_grammar.grammar.insert(rule);
    }
    std::set<std::string> eps_rules = getEpsRules(new_grammar);
    for (auto rule: eps_rules) {
        for (auto rules: new_grammar.grammar) {
            for (auto r: rules.second) {
                std::vector<int> idxes = getIndex(rule, r);
                std::vector<std::vector<int>> subsets;
                std::vector<int> subset;
                int idx = 0;
                getSubsets(idxes, subsets, subset, idx);
                for (auto sub: subsets) {
                    std::vector<std::string> newRule;
                    if (!sub.empty()) {
                        newRule = getNewRule(sub, r);
                        if (!newRule.empty())
                            new_grammar.grammar[rules.first].insert(newRule);
                    }
                }
            }
        }
    }
    if (eps_rules.contains(grammar.ZeroState)) {
        new_grammar.ZeroState = grammar.ZeroState + "'";
        std::vector<std::string> s = {grammar.ZeroState};
        std::vector<std::string> e = {"e"};
        new_grammar.grammar[new_grammar.ZeroState].insert(s);
        new_grammar.grammar[new_grammar.ZeroState].insert(e);
    }else{
        new_grammar.ZeroState = grammar.ZeroState;
    }
    return new_grammar;
}


Grammar getGrammar(const std::string &grammarPath) {
    std::ifstream grammar(grammarPath);
    std::string rule;
    grammar_t ans;
    while (std::getline(grammar, rule)) {
        rule.erase(std::remove_if(rule.begin(), rule.end(), ::isspace),
                   rule.end());
        std::string left = rule.substr(0, rule.find("->"));
        std::string right = rule.substr(rule.find("->") + 2, rule.length() - 1);
        splitRight(ans[left], right);
    }

    return {"S", ans};
}

void splitRight(std::set<std::vector<std::string>> &rule, std::string &right) {
    size_t pos;
    std::string token;
    while ((pos = right.find('|')) != std::string::npos) {
        token = right.substr(0, pos);
        std::vector<std::string> v;
        for (auto letter: token) {
            v.emplace_back(1, letter);
        }
        rule.insert(v);
        right.erase(0, pos + 1);
    }
    std::vector<std::string> v;
    for (auto letter: right) {
        v.emplace_back(1, letter);
    }
    rule.insert(v);
}
