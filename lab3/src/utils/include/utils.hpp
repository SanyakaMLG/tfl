#ifndef LAB3_UTILS_HPP
#define LAB3_UTILS_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include "Earley.hpp"


using namespace EarleyNames;

Grammar getGrammar(const std::string &grammar);

void splitRight(std::set<std::vector<std::string>> &rule, std::string &right);

void getRulePrefix(Grammar &grammar,
                   const std::string &left,
                   const std::set<std::vector<std::string>> &right);

Grammar getPrefixLanguageGrammar(Grammar &grammar);

void printGrammar(Grammar &grammar);

void reverseRule(Grammar &grammar, const std::string &left,
                 std::set<std::vector<std::string>> &right);

Grammar reverseGrammar(Grammar &grammar);

std::set<std::string> getEpsRules(Grammar &grammar);

Grammar deleteEpsRules(Grammar &grammar);

std::vector<std::string> generateStrings(int length, std::set<char> &alphabet);
#endif //LAB3_UTILS_HPP
