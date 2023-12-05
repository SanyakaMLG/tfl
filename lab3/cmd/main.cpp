#include <iostream>
#include <sstream>

#include "OracleModule.hpp"
#include "LStar.hpp"
#include "ProgramOptions.hpp"
/*
 Правила записи грамматики:
 Нетерминалы - заглавные буквы
 Терминалы - строчные буквы или другие символы(за исключением специальных)*/
void belongs(OracleModule &Oracle, std::string &word){
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
}


void printGrammars(OracleModule &Oracle){
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
}

std::vector<std::string> splitString(std::string &str){
    std::string part;
    std::vector<std::string> ans;
    std::stringstream splitable(str);
    while (std::getline(splitable, part,' ')){
        ans.push_back(part);
    }
    for (auto &p: ans) {
        if (p == "(eps)")
            p = "";
    }
    return ans;
}

std::string concat(std::vector<std::string>& partition){
    std::string ans;
    for(const auto& part:partition){
        ans += part;
    }
    return ans;
}


void test(OracleModule& Oracle, std::string& path){
    std::string acc;
    std::ifstream file(path);
    std::vector<std::string> partition;
    std::set<char> alphabet = Oracle.withoutEpsGrammar.getAlphabet();
    while(std::getline(file, acc)){
       partition = splitString(acc);
       if(ProgramOptions::log()){
            std::string word = concat(partition);
            belongs(Oracle, word);
       }

        LStar algo(Oracle, alphabet, partition, 5, 3, 200);

        DFA prefix = algo.get_language("prefix");
//        std::cout<<"Prefix Language:\n";
//        prefix.printDot();

        DFA inf = algo.get_language("infix");
        std::cout<<"Infix Language:\n";
        inf.printDot();

        DFA suf = algo.get_language("suffix");
//        std::cout<<"Suffix Language:\n";
//        suf.printDot();

        auto counter = algo.get_counter_DFAs(prefix, suf);
//        std::cout << "Counter prefix:\n";
//        counter[0].printDot();
//        std::cout << "Counter suffix:\n";
//        counter[1].printDot();

        if (counter.empty()){
            std::cout<<"Prefix Language:\n";
            prefix.printDot();
            std::cout<<"Suffix Language:\n";
            suf.printDot();
        }else{
            auto invertedCounterPrefix = counter[0].invert();
            auto invertedCounterSuffix = counter[1].invert();
            auto prefixIntersect = intersect(prefix,invertedCounterPrefix);
            auto suffixIntersect = intersect(suf, invertedCounterSuffix);
            std::cout<<"Prefix Language:\n";
            prefixIntersect.printDot();
            std::cout<<"Suffix Language:\n";
            suffixIntersect.printDot();
        }
    }
}


int main(int argc, char* argv[]) {
    ProgramOptions::parse(argc, argv);
    std::string grammarPath("grammar");
    std::cout<<"making oracles\n";
    auto Oracle = OracleModule(grammarPath);
    if (ProgramOptions::log()) {
        printGrammars(Oracle);
    }
    std::vector<std::string> partition;
    if(ProgramOptions::need_file()) {
        std::string path(ProgramOptions::str());
        test(Oracle, path);
    }
//
//    ObservationTable table(Oracle, "infix", a_alph, 5, partition);
//    table.print_table();
//
//    table.convert_to_dfa().printDot();
//    table.add_counterexample("aa");
//    table.print_table();
//    table.add_counterexample("baa");
//    table.print_table();
//
//    table.convert_to_dfa().printDot();
    return 0;
}
