#ifndef LAB3_LSTAR_HPP
#define LAB3_LSTAR_HPP
#include "OracleModule.hpp"
#include "DFA.hpp"
#include "ObservationTable.h"
#include <unordered_map>
#include <unordered_set>

class LStar {
private:
    OracleModule oracle;
    std::set<char> alphabet;
    std::vector<std::string> partition;
    int limit_pump;
    int reg_limit;
    int eq_limit;
    DFA get_language_in_alphabet(std::string mode, std::set<char> &_alphabet);
public:
    LStar(OracleModule _oracle, std::set<char> _alphabet, std::vector<std::string> _partition,
          int _limit_pump, int _reg_limit, int _eq_limit):
    oracle(_oracle), alphabet(_alphabet), partition(_partition),
    limit_pump(_limit_pump), reg_limit(_reg_limit), eq_limit(_eq_limit) {}

    DFA get_language(std::string mode);
    std::vector<DFA> get_counter_DFAs(DFA&, DFA&);
    bool check_compatibility(DFA&, DFA&);
};

#endif //LAB3_LSTAR_HPP
