#include <iostream>

#include "OracleModule.hpp"
#include "ObservationTable.h"
/*
 Правила записи грамматики:
 Нетерминалы - заглавные буквы
 Терминалы - строчные или другие символы(за исключением специальных)*/

std::set<std::set<char>> including_alphabet(std::set<char> full_alphabet, std::set<char> tmp_alphabet) {
    std::set<std::set<char>> res = {tmp_alphabet};
    std::set<char> not_in_alph;
    std::set_difference(full_alphabet.begin(), full_alphabet.end(),
                        tmp_alphabet.begin(), tmp_alphabet.end(),
                        std::inserter(not_in_alph, not_in_alph.begin()));

    while (!res.contains(full_alphabet)) {
        for (auto el: res) {
            for (auto c: not_in_alph) {
                std::set<char> tmp = el;
                tmp.insert(c);
                res.insert(tmp);
            }
        }
    }
    return res;
}


int main() {
    std::string grammarPath("grammar");
    auto Oracle = OracleModule(grammarPath);
    std::cout << "OriginalGrammar" << '\n';
    printGrammar(Oracle.originalGrammar);
    std::cout << "OriginalGrammar DEl_EPS" << '\n';
    printGrammar(Oracle.withoutEpsGrammar);
    std::cout << '\n' << "PrefixGrammar" << '\n';
    printGrammar(Oracle.prefixGrammar);
    std::cout << '\n' << "ReversedGrammar" << '\n';
    printGrammar(Oracle.reversedGrammar);
    std::cout << '\n' << "PostfixGrammar" << '\n';
    printGrammar(Oracle.postfixGrammar);
    std::cout << '\n' << "InfixGrammar" << '\n';
    printGrammar(Oracle.infixGrammar);
    std::string word = "((()))())";
    std::cout << "Candidate: " << word << '\n';
    bool ans = Oracle.inLanguage(word);
    bool pref = Oracle.inPrefixLanguage(word);
    bool infix = Oracle.inInfixLanguage(word);
    bool postfix = Oracle.inPostfixLanguage(word);
    bool reversed = Oracle.inReversedLanguage(word);
    if (infix) {
        std::cout << word << " ∈ Infix(L)" << '\n';
    } else {
        std::cout << word << " ∉ Infix(L)" << '\n';
    }

    if (postfix) {
        std::cout << word << " ∈ Postfix(L)" << '\n';
    } else {
        std::cout << word << " ∉ Postfix(L)" << '\n';
    }

    if (reversed) {
        std::cout << word << " ∈ Reversed(L)" << '\n';
    } else {
        std::cout << word << " ∉ Reversed(L)" << '\n';
    }

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

    std::set<char> alphabet = {'a', 'b'};
    std::vector<std::string> partition = {"aa", "a", "aabb", "b", "bb"};
    ObservationTable table(Oracle, "prefix", alphabet, 5, partition);
    table.print_table();

//    table.add_counterexample("aa");
//    table.add_counterexample("aaba");
//
//    table.add_counterexample("ab");
    table.print_table();
//
//
    DFA dfa = table.convert_to_dfa();
    dfa.deleteTrap();
    dfa.printDot();

//    std::vector<std::string> strings = generateStrings(5, alphabet);
//    for (auto el: strings) {
//        std::cout << el << std::endl;
//    }
    return 0;
}
