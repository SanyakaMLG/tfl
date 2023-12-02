#ifndef LAB3_GRAMMAR_HPP
#define LAB3_GRAMMAR_HPP
#include <iostream>

namespace EarleyNames {
    using grammar_t = std::map<std::string, std::set<std::vector<std::string>>>;
    using state_t = std::tuple<std::string, std::vector<std::string>,size_t, int>;
    using set_of_states_t = std::set<state_t>;
    using situation_map_t = std::map<std::string, set_of_states_t>;
    using s_vector = std::vector<std::string>;
}

using namespace EarleyNames;
class Grammar{
public:
    std::string ZeroState;
    grammar_t grammar;
    std::set<char> getAlphabet();
};
#endif //LAB3_GRAMMAR_HPP
