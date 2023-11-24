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
    std::vector<std::string> partition;
public:
    LStar(OracleModule _oracle, std::set<char> _alphabet, std::vector<std::string> _partition):
    oracle(_oracle), alphabet(_alphabet), partition(_partition) {}

    DFA get_language(std::string mode);
};

#endif //LAB3_LSTAR_HPP
