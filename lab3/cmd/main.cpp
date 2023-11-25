#include <iostream>

#include "OracleModule.hpp"
#include "LStar.hpp"
/*
 Правила записи грамматики:
 Нетерминалы - заглавные буквы
 Терминалы - строчные или другие символы(за исключением специальных)*/

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
    std::set<char> a_alph = {'a'};
    std::vector<std::string> partition = {"aa", "a", "aabb", "b", "bb"};

    LStar algo(Oracle, alphabet, partition, 5, 2, 1000);

    DFA dfa = algo.get_language("prefix");

    dfa.printDot();
//
//    ObservationTable table(Oracle, "prefix", alphabet, 5, partition);
//    table.print_table();
//    table.add_counterexample("aa");
//    table.print_table();
//    table.add_counterexample("baa");
//    table.print_table();
//
//    table.convert_to_dfa().printDot();
    return 0;
}
