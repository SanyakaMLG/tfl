#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include "Earley.hpp"

/*
 Правила записи грамматики:
 Нетерминалы - заглавные буквы
 Терминалы - строчные или другие символы(за исключением специальных)*/


std::map<std::string, std::set<std::vector<std::string>>> getGrammar(const std::string &grammar);

void splitRight(std::set<std::vector<std::string>> &rule, std::string &right);


void getRulePrefix(std::map<std::string, std::set<std::vector<std::string>>> &grammar,
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
            grammar[prefix_left].insert(new_rule);
        }
    }
}

std::map<std::string, std::set<std::vector<std::string>>>
getPrefixLanguageGrammar(std::map<std::string, std::set<std::vector<std::string>>> &grammar) {
    std::map<std::string, std::set<std::vector<std::string>>> prefix_grammar;
    for (const auto &rule: grammar) {
        prefix_grammar[rule.first] = rule.second;
    }
    prefix_grammar["S^"].insert(std::vector<std::string>(1, "eps"));
    for (const auto &rule: grammar) {
        getRulePrefix(prefix_grammar, rule.first, rule.second);
    }
    return prefix_grammar;
}

void printGrammar(std::map<std::string, std::set<std::vector<std::string>>> &grammar) {
    for (const auto &rule: grammar) {
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

void reverseRule(std::map<std::string, std::set<std::vector<std::string>>> &grammar, const std::string &left,
                 std::set<std::vector<std::string>> &right) {
    for (auto &item: right) {
        std::vector<std::string> reverse(item.size());
        size_t i = item.size() - 1;
        for (auto &let: item) {
            reverse[i] = let;
            i--;
        }
        grammar[left].insert(reverse);
    }
}

std::map<std::string, std::set<std::vector<std::string>>>
reverseGrammar(std::map<std::string, std::set<std::vector<std::string>>> &grammar) {
    std::map<std::string, std::set<std::vector<std::string>>> reverse_grammar;
    for (auto &rule: grammar) {
        reverseRule(reverse_grammar, rule.first, rule.second);
    }
    return reverse_grammar;
}


std::set<std::string> getEpsRules(std::map<std::string, std::set<std::vector<std::string>>> &grammar) {
    std::set<std::string> ans;
    for (const auto &rules: grammar) {
        if (rules.second.contains(std::vector<std::string>(1, "e"))) {
            ans.insert(rules.first);
            grammar[rules.first].erase(std::vector<std::string>(1, "e"));
        }
    }
    bool changed = true;
    while (changed) {
        size_t len = ans.size();
        for (const auto &rules: grammar) {
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

void getSubsets(std::vector<int> &idxes, std::vector<std::vector<int>>& subsets, std::vector<int> &subset, int idx) {
    subsets.push_back(subset);
    for (int i = idx; i < idxes.size(); i++) {
        subset.push_back(idxes[i]);
        getSubsets(idxes, subsets, subset, i + 1);
        subset.pop_back();
    }
}
std::vector<std::string> getNewRule(std::vector<int>& idx, std::vector<std::string>& rule){
    std::vector<std::string> newRule = rule;
    for(int i:idx){
        newRule.erase(newRule.begin()+i);
    }
    return newRule;
}
std::map<std::string, std::set<std::vector<std::string>>>  deleteEpsRules(std::map<std::string, std::set<std::vector<std::string>>> &grammar) {
    std::set<std::string> eps_rules = getEpsRules(grammar);
    std::map<std::string, std::set<std::vector<std::string>>> new_grammar;
    for(auto rule:grammar){
        new_grammar.insert(rule);
    }
    for (auto rule: eps_rules) {
        for (auto rules: new_grammar) {
            for (auto r: rules.second) {
                std::vector<int> idxes = getIndex(rule, r);
                std::vector<std::vector<int>> subsets;
                std::vector<int> subset;
                int idx = 0;
                getSubsets(idxes, subsets, subset, idx);
                for(auto sub: subsets){
                    std::vector<std::string> newRule;
                    if(!sub.empty()){
                        newRule = getNewRule(sub, r);
                        if(!newRule.empty())
                            new_grammar[rules.first].insert(newRule);
                    }
                }
            }
        }
    }
    if(eps_rules.contains("S")){
        std::vector<std::string> s = {"S"};
    std::vector<std::string> e = {"e"};
        new_grammar["S''"].insert(s);
        new_grammar["S''"].insert(e);
    }
    return new_grammar;
}

int main() {
    std::string grammarPath("grammar");
    auto grammar = getGrammar(grammarPath);
    std::cout << "OriginalGrammar" << '\n';
    printGrammar(grammar);
    auto new_grammar = deleteEpsRules(grammar);
    std::cout << "OriginalGrammar DEl_EPS" << '\n';
    printGrammar(new_grammar);
//    std::cout << ^\n^ << "ReversedGrammar" << ^\n^;
//    auto reversedGrammar = reverseGrammar(grammar);
//    printGrammar(reversedGrammar);
    auto prefixGrammar = getPrefixLanguageGrammar(grammar);
    std::cout << '\n' << "PrefixGrammar" << '\n';
    printGrammar(prefixGrammar);
    std::string word = "(a+a";
    std::cout << "Candidate: " << word << '\n';
    bool ans = Earley(grammar, word);
    bool pref = Earley(prefixGrammar, word);
    if (ans) {
        std::cout << word << " ∈ L" << '\n';
    } else {
        std::cout << word << " ∉ L" << '\n';
    }
    if (pref) {
        std::cout << word << " ∈ Prefix(L)" << '\n';
    } else {
        std::cout << word << " ∉ Prefix(L)" << '\n';
    }
    return 0;
}

std::map<std::string, std::set<std::vector<std::string>>> getGrammar(const std::string &grammarPath) {
    std::ifstream grammar(grammarPath);
    std::string rule;
    std::map<std::string, std::set<std::vector<std::string>>> ans;
    while (std::getline(grammar, rule)) {
        rule.erase(std::remove_if(rule.begin(), rule.end(), ::isspace),
                   rule.end());
        std::string left = rule.substr(0, rule.find("->"));
        std::string right = rule.substr(rule.find("->") + 2, rule.length() - 1);
        splitRight(ans[left], right);
    }
    return ans;
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
