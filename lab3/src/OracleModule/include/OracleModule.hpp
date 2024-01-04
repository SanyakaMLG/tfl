#ifndef LAB3_ORACLEMODULE_HPP
#define LAB3_ORACLEMODULE_HPP
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "utils.hpp"

using namespace EarleyNames;
class OracleModule {

public:
    Grammar originalGrammar;
    Grammar withoutEpsGrammar;
    Grammar reversedGrammar;
    Grammar prefixGrammar;
    Grammar postfixGrammar;
    Grammar infixGrammar;
    explicit OracleModule(std::string& filepath);
    bool inLanguage(std::string&);
    bool inReversedLanguage(std::string&);
    bool inPrefixLanguage(std::string&);
    bool inPostfixLanguage(std::string&);
    bool inInfixLanguage(std::string&);
};


#endif //LAB3_ORACLEMODULE_HPP
