#include <iostream>
#include <set>
#include <vector>
#include <map>
#include "Grammar.hpp"
namespace EarleyNames {
    using grammar_t = std::map<std::string, std::set<std::vector<std::string>>>;
    using state_t = std::tuple<std::string, std::vector<std::string>,size_t, int>;
    using set_of_states_t = std::set<state_t>;
    using situation_map_t = std::map<std::string, set_of_states_t>;
    using s_vector = std::vector<std::string>;
}

using namespace EarleyNames;

void addZeroState(Grammar &grammar);

std::vector<situation_map_t> init(size_t size, Grammar &grammar);

std::pair<state_t, std::string> createStateScan(state_t &old_state);

void scan(std::vector<situation_map_t> &situations, size_t j, std::string &word);

std::pair<state_t, std::string> createStateComplete(state_t &old_state, size_t j);


bool complete(std::vector<situation_map_t> &situations, size_t j);

bool createStatePredict(situation_map_t &situation, const std::string &left,const s_vector &rule,
                        size_t j);

bool predict(std::vector<situation_map_t> &situations, size_t j, Grammar &grammar);

bool Earley(Grammar grammar, std::string &word);