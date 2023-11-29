#include "LStar.hpp"
#include "utils.hpp"

struct cmpBySetSize {
    bool operator()(const std::set<char> &a, const std::set<char> &b) const {
        if (a.size() < b.size()) {
            return true;
        } else if (a.size() == b.size()) {
            for (auto it_1 = a.begin(), it_2 = b.begin(); it_1 != a.end(); it_1++, it_2++) {
                if (*it_1 < *it_2)
                    return true;
                else if (*it_1 > *it_2)
                    return false;
            }
        }

        return false;
    }
};

std::set<std::set<char>, cmpBySetSize> get_subsets(std::set<char> &set) {
    int n = set.size();
    std::vector<char> elements(set.begin(), set.end());
    std::set<std::set<char>, cmpBySetSize> res;
    for (int i = 1; i < (1 << n); i++) {
        std::set<char> subset;
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) {
                subset.insert(elements[j]);
            }
        }

        res.insert(subset);
    }
    return res;
}

std::set<std::set<char>, cmpBySetSize> get_supersets(const std::set<char> &set, const std::set<char> &desired_subset) {
    std::set<std::set<char>, cmpBySetSize> res;
    std::vector<char> elements(set.begin(), set.end());
    int n = elements.size();
    for (int i = 0; i < (1 << n); i++) {
        std::set<char> subset;
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) {
                subset.insert(elements[j]);
            }
        }

        bool contains_desired_subset = true;
        for (int element : desired_subset) {
            if (subset.find(element) == subset.end()) {
                contains_desired_subset = false;
                break;
            }
        }

        if (contains_desired_subset) {
            res.insert(subset);
        }
    }

    return res;
}


DFA LStar::get_language(std::string mode) {
    int idx = mode == "prefix" ? 0 : mode == "suffix" ? 4 : 2;
    int max_dfa_count = partition[idx].length() + reg_limit;
    auto alphabets = get_subsets(alphabet);
    std::set<std::set<char>> blocked;
    DFA dfa(alphabet);
    DFA last_reg = dfa;

    for (auto alph: alphabets) {

        if (!blocked.contains(alph))
            dfa = get_language_in_alphabet(mode, alph);

        if (dfa.getSize() > max_dfa_count || dfa.getSize() == 0) {
            auto to_block = get_supersets(alphabet, alph);
            for (auto el: to_block)
                blocked.insert(el);
        } else {
            last_reg = dfa;
        }
    }

    return last_reg;
}

DFA LStar::get_language_in_alphabet(std::string mode, std::set<char> &_alphabet) {
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
        end = 20;
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

std::vector<DFA> LStar::get_counter_DFAs(DFA &prefix, DFA &suffix) {
    std::set<std::tuple<std::string, std::string>> counter_examples;
    std::vector<DFA> res;

    for (int i = 0; i < 200; i++) {
        auto pref = prefix.getRandomString();
        for (int j = 0; j < 200; j++) {
            auto suf = suffix.getRandomString();

            bool is_counter = false;
            for (int pump = 0; pump < limit_pump; pump++) {
                std::string to_check = pref;
                for (int k = 0; k < pump; k++)
                    to_check.append(partition[2]);
                to_check.append(partition[3]);
                for (int k = 0; k < pump; k++)
                    to_check.append(partition[4]);
                to_check.append(suf);
                if (!oracle.inLanguage(to_check)) {
                    is_counter = true;
                    break;
                }
            }

            if (is_counter)
                counter_examples.insert(std::make_tuple(pref, suf));
        }
    }

    auto pref_alphabet = prefix.getAlphabet();
    auto suf_alphabet = suffix.getAlphabet();
    if (counter_examples.size() >= prefix.getSize() + suffix.getSize()) {
        ObservationTable pref_table(oracle, "prefix", pref_alphabet, limit_pump, partition);
        ObservationTable suf_table(oracle, "suffix", suf_alphabet, limit_pump, partition);
        for (auto &ex: counter_examples) {
            pref_table.add_counterexample(std::get<0>(ex));
            suf_table.add_counterexample(std::get<1>(ex));
        }

        auto pref_DFA = pref_table.convert_to_dfa();
        pref_DFA.deleteTrap();

        auto suf_DFA = suf_table.convert_to_dfa();
        suf_DFA.deleteTrap();

        res.push_back(pref_DFA);
        res.push_back(suf_DFA);
    }

    // may be empty
    return res;
}
