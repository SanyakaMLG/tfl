#include "OracleModule.hpp"

bool OracleModule::inLanguage(std::string& w) {
    return Earley(withoutEpsGrammar, w);
}

bool OracleModule::inReversedLanguage(std::string & w) {
    return Earley(reversedGrammar, w);
}

bool OracleModule::inPrefixLanguage(std::string & w) {
    return Earley(prefixGrammar, w);
}

bool OracleModule::inPostfixLanguage(std::string & w) {
    return Earley(postfixGrammar, w);
}

bool OracleModule::inInfixLanguage(std::string & w) {
    return Earley(infixGrammar, w);
}

OracleModule::OracleModule(std::string &filepath) {
    originalGrammar = getGrammar(filepath);
    withoutEpsGrammar = deleteEpsRules(originalGrammar);
    reversedGrammar = reverseGrammar(withoutEpsGrammar);
    prefixGrammar = getPrefixLanguageGrammar(withoutEpsGrammar);
    auto reversedPostfix = getPrefixLanguageGrammar(reversedGrammar);
    postfixGrammar = reverseGrammar(reversedPostfix);
    auto reversedPrefix = reverseGrammar(prefixGrammar);
    auto reversedInfix = getPrefixLanguageGrammar(reversedPrefix);
    reversedInfix = deleteEpsRules(reversedInfix);
    infixGrammar = reverseGrammar(reversedInfix);


}
