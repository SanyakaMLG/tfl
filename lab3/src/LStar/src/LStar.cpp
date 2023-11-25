#include "LStar.hpp"
#include "utils.hpp"

// to rewrite???????
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


DFA LStar::get_language(std::string mode) {
    int idx = mode == "prefix" ? 0 : mode == "suffix" ? 4 : 2;
    int max_dfa_count = partition[idx].length() + reg_limit;
    std::set<char> alph;
    std::set<std::set<char>> blocked;
    for (auto c: partition[idx])
        alph.insert(c);
    DFA dfa = get_language_in_alphabet(mode, alph);

    // if first dfa is irregular then return empty dfa
    if (dfa.getSize() > max_dfa_count || dfa.getSize() == 0) {
        dfa.clear();
        return dfa;
    }

    DFA last_reg = dfa;

    // need to search deeper
    bool flag = true;

    while (flag) {
        flag = false;
        for (auto c: alphabet) {
            std::set<char> tmp_alphabet = alph;
            if (tmp_alphabet.contains(c))
                continue;
            tmp_alphabet.insert(c);
            if (!blocked.contains(tmp_alphabet)) {
                dfa = get_language_in_alphabet(mode, tmp_alphabet);

                if (dfa.getSize() > max_dfa_count || dfa.getSize() == 0) {
                    blocked.insert(tmp_alphabet);
                    auto res = including_alphabet(alphabet, tmp_alphabet);
                    for (auto el: res)
                        blocked.insert(el);
                } else {
                    last_reg = dfa;
                    alph = tmp_alphabet;
                    flag = true;
                    break;
                }
            }
        }
    }

    return last_reg;
}

DFA LStar::get_language_in_alphabet(std::string mode, std::set<char> &_alphabet) {
    if (_alphabet.contains('a') && _alphabet.contains('b')) {
        int k = 15421;
    }
    ObservationTable table(oracle, mode, _alphabet, limit_pump, partition);
    DFA dfa = table.convert_to_dfa();
    dfa.deleteTrap();
    std::vector<std::string> strings;

    int i = 0;
    while(strings.size() < eq_limit) {
        std::vector<std::string> generated = generateStrings(i, _alphabet);
        for (auto gen: generated)
            strings.push_back(gen);
        i++;
    }

    bool in_dfa, in_oracle;
    int end;

    if (_alphabet.size() == 1)
        end = 15;
    else
        end = strings.size();

    for (i = 0; i < end; i++) {
        in_dfa = dfa.checkString(strings[i]);
        in_oracle = table.check_string(strings[i]);
        if (in_oracle && !in_dfa || in_dfa && !in_oracle) {
            table.add_counterexample(strings[i]);
            dfa = table.convert_to_dfa();
            dfa.deleteTrap();
        }
    }

    return dfa;
}
