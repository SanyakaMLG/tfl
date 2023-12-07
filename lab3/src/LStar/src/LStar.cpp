#include <stack>
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
        for (int element: desired_subset) {
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
    if (mode == "infix")
        int skjdgkjsdgjhk=1;
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
    while (strings.size() < eq_limit) {
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

/*
DFA get_counter_DFA2(OracleModule oracle, std::string mode, std::set<char> &alphabet, int limit_pump,
                    std::vector<std::string> partition, std::set<std::string> &examples) {
    CounterTable counter_table(oracle, mode, alphabet, limit_pump, partition, examples);
    DFA counter_dfa = counter_table.convert_to_dfa();
    counter_dfa.deleteTrap();
    std::vector<std::string> strings;

    int i = 0;
    while (strings.size() < 256) {
        std::vector<std::string> generated = generateStrings(i, alphabet);
        for (auto gen: generated)
            strings.push_back(gen);
        i++;
    }

    bool in_dfa, in_oracle;
    int end;

    if (alphabet.size() == 1)
        end = 20;
    else
        end = strings.size();

    for (i = 0; i < end; i++) {
        in_dfa = counter_dfa.checkString(strings[i]);
        in_oracle = counter_table.check_string(strings[i]);
        if (in_oracle && !in_dfa || in_dfa && !in_oracle) {
            counter_table.add_counterexample(strings[i]);
            counter_dfa = counter_table.convert_to_dfa();
            counter_dfa.deleteTrap();
        }
    }

    return counter_dfa;
}
 */

DFA get_counter_DFA(OracleModule oracle, std::string mode, std::set<char> &alphabet, int limit_pump,
                    std::vector<std::string> partition, DFA pref, DFA suf, std::vector<std::set<std::string>> full_checked) {
    CounterTable counter_table(oracle, mode, alphabet, limit_pump, partition, pref, suf, full_checked);
    DFA counter_dfa = counter_table.convert_to_dfa();
    counter_dfa.deleteTrap();
    std::vector<std::string> strings;

    if (alphabet.size() == 0) {
        return counter_dfa;
    }

    int i = 0;
    while (strings.size() < 256) {
        std::vector<std::string> generated = generateStrings(i, alphabet);
        for (auto gen: generated)
            strings.push_back(gen);
        i++;
    }

    bool in_dfa, in_oracle;
    int end;

    if (alphabet.size() == 1)
        end = 5;
    else
        end = strings.size();

    for (i = 0; i < end; i++) {
        in_dfa = counter_dfa.checkString(strings[i]);
        in_oracle = counter_table.check_string(strings[i]);
        if (in_oracle && !in_dfa || in_dfa && !in_oracle) {
            counter_table.add_counterexample(strings[i]);
            counter_dfa = counter_table.convert_to_dfa();
            counter_dfa.deleteTrap();
        }
    }

    return counter_dfa;
}


//std::string searchFromTo(DFA& dfa, int from, std::set<int>& to){
//    std::set<int> visited;
//    std::stack<std::pair<int, std::string>> stack;
//    stack.emplace(from, "");
//    while(!stack.empty()){
//        auto cur = stack.top();
//        visited.insert(cur.first);
//        stack.pop();
//        if (to.contains(cur.first)){
//            return cur.second;
//        }
//        for (auto child: dfa.transitions_map[cur.first]){
//            if(!visited.contains(child.second)){
//                stack.emplace(child.second, cur.second+child.first);
//            }
//        }
//    }
//}
//std::string getWord(DFA& dfa, int i){
//    std::set<int> set={i};
//    auto middle = searchFromTo(dfa, 0, set);
//    auto finalStates = dfa.getFinalStates();
//    return middle + searchFromTo(dfa, i,finalStates);
//}
//
//
//std::set<std::string> getWords(DFA &dfa) {
//    std::set<std::string> words;
//    for (int i = 0; i < dfa.getSize(); i++) {
//        auto word = getWord(dfa, i);
//        words.insert(word);
//    }
//    return words;
//}

std::vector<DFA> LStar::get_counter_DFAs(DFA &prefix, DFA &suffix) {
    std::set<std::tuple<std::string, std::string>> counter_examples;
    std::set<std::string> checked_prefix, checked_suffix;
    std::vector<std::set<std::string>> full_checked(2);
    std::vector<DFA> res;

    for (int i = 0; i < 60; i++) {

        auto pref = prefix.getRandomString();

        if (checked_prefix.contains(pref))
            continue;
        else
            checked_prefix.insert(pref);

        for (int j = 0; j < 15; j++) {

            auto suf = suffix.getRandomString();

            if (checked_suffix.contains(suf))
                continue;
            else
                checked_suffix.insert(suf);

            for (int pump = 0; pump < limit_pump; pump++) {
                std::string to_check = pref;
                for (int k = 0; k < pump; k++)
                    to_check.append(partition[1]);
                to_check.append(partition[2]);
                for (int k = 0; k < pump; k++)
                    to_check.append(partition[3]);
                to_check.append(suf);
                if (!oracle.inLanguage(to_check)) {
                    counter_examples.insert(std::make_tuple(pref, suf));
                    full_checked[0].insert(pref);
                    full_checked[1].insert(suf);
                    break;
                }
            }
        }

        checked_suffix.clear();
    }

    // empty return if not enough counter examples
    if (counter_examples.size() < prefix.getSize() + suffix.getSize())
        return res;

    std::set<char> pref_alphabet, suf_alphabet;
    for (auto &ex: counter_examples) {
        for (auto c: std::get<0>(ex))
            pref_alphabet.insert(c);

        for (auto c: std::get<1>(ex))
            suf_alphabet.insert(c);
    }
/*
    // нужно ли строить эти ДКА в таких алфавитах??
    auto antipref = get_language_in_alphabet("antiprefix", pref_alphabet);
    auto antisuf = get_language_in_alphabet("antisuffix", suf_alphabet);

    antipref = intersect(antipref, suffix);
    antisuf = intersect(antisuf, prefix);

    // нужно получить строки ck из автоматов antipref и antisuf
    // suffix_examples from antipref, prefix_examples from antisuf
    // ....
    // получили строки
    std::set<std::string> suffix_examples = getWords(antipref);
    std::set<std::string> prefix_examples = getWords(antisuf);

    auto counter_prefix = get_counter_DFA(oracle, "prefix", pref_alphabet, limit_pump, partition, suffix_examples);
    auto counter_suffix = get_counter_DFA(oracle, "suffix", suf_alphabet, limit_pump, partition, prefix_examples);
*/

    auto counter_prefix = get_counter_DFA(oracle, "prefix", pref_alphabet, limit_pump, partition, prefix, suffix, full_checked);
    auto counter_suffix = get_counter_DFA(oracle, "suffix", suf_alphabet, limit_pump, partition, prefix, suffix, full_checked);
    res.push_back(counter_prefix);
    res.push_back(counter_suffix);

    return res;
}

bool LStar::check_compatibility(DFA &prefix, DFA &suffix) {
    std::set<std::string> checked_prefix, checked_suffix;

    for (int i = 0; i < 20; i++) {

        auto pref = prefix.getRandomString();

        if (checked_prefix.contains(pref))
            continue;
        else
            checked_prefix.insert(pref);

        for (int j = 0; j < 15; j++) {

            auto suf = suffix.getRandomString();

            if (checked_suffix.contains(suf))
                continue;
            else
                checked_suffix.insert(suf);

            for (int pump = 0; pump < limit_pump; pump++) {
                std::string to_check = pref;
                for (int k = 0; k < pump; k++)
                    to_check.append(partition[1]);
                to_check.append(partition[2]);
                for (int k = 0; k < pump; k++)
                    to_check.append(partition[3]);
                to_check.append(suf);
                if (!oracle.inLanguage(to_check)) {
                    return false;
                }
            }
        }

        checked_suffix.clear();
    }

    return true;
}
