#ifndef LAB3_LSTAR_HPP
#define LAB3_LSTAR_HPP
#include "OracleModule.hpp"
#include "DFA.hpp"
#include "ObservationTable.h"
#include <unordered_map>

class LStar {
private:
    OracleModule oracle;
    std::set<char> alphabet;
public:
    LStar(OracleModule _oracle, std::set<char> _alphabet): oracle(_oracle), alphabet(_alphabet) {}

    DFA get_language_in_alphabet(std::set<char> alphabet);
};

#endif //LAB3_LSTAR_HPP
