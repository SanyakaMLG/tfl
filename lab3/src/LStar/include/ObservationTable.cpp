#include "ObservationTable.h"

bool ObservationTable::is_closed() {
    std::set<std::vector<bool>> diff;
    std::set_difference(extended_rows.begin(), extended_rows.end(), rows.begin(), rows.end(),
                        std::inserter(diff, diff.begin()));
    return diff.empty();
}

void ObservationTable::make_closure() {
    for (auto p: extended_prefix) {
        if (!rows.contains(p.second)) {
            prefix[p.first] = p.second;
            rows.insert(p.second);
        }
    }
    extended_prefix.clear();
    extended_rows.clear();
    for (auto c: alphabet) {
        for (auto p : prefix) {
            if (!prefix.contains(p.first + c)) {
                std::string str = p.first + c;
                std::vector<bool> tmp;
                for (auto suf : suffix) {
                    bool ans;
                    if (mode == "prefix")
                        ans = oracle.inPrefixLanguage(str);
                    else if (mode == "suffix")
                        ans = oracle.inPostfixLanguage(str);
                    else
                        ans = oracle.inInfixLanguage(str);
                    tmp.push_back(ans);
                }
                extended_prefix[str] = tmp;
                extended_rows.insert(tmp);
            }
        }
    }
}

bool ObservationTable::is_consistent(std::string &pref1, std::string &pref2, char &suf) {
    for (auto it = begin(prefix); it != end(prefix); it++) {
        for (auto it_2 = ++it; it_2 != end(prefix); it_2++) {
            if (std::equal(it->second.begin(), it->second.end(), it_2->second.begin(), it_2->second.end())) {
                for (auto c: alphabet) {
                    std::vector<bool> vec1, vec2;
                    if (prefix.contains(it->first + c))
                        vec1 = prefix[it->first + c];
                    else
                        vec1 = extended_prefix[it->first + c];

                    if (prefix.contains(it_2->first + c))
                        vec2 = prefix[it_2->first + c];
                    else
                        vec2 = extended_prefix[it_2->first + c];

                    if (!std::equal(vec1.begin(), vec1.end(), vec2.begin(), vec2.end())) {
                        suf = c;
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void ObservationTable::make_consistence(std::string &pref1, std::string &pref2, char &suf) {
    std::vector<bool> p1, p2;
    std::string full_suf;
    if (prefix.contains(pref1 + suf))
        p1 = prefix[pref1 + suf];
    else
        p1 = extended_prefix[pref1 + suf];

    if (prefix.contains(pref2 + suf))
        p2 = prefix[pref2 + suf];
    else
        p2 = extended_prefix[pref2 + suf];

    for (int i = 0; i < p1.size(); i++) {
        if (p1[i] != p2[i]) {
            full_suf = suf + suffix[i];
        }
    }

    suffix.push_back(full_suf);
    for (auto p: prefix) {
        bool ans;
        std::string str = p.first + full_suf;
        if (mode == "prefix")
            ans = oracle.inPrefixLanguage(str);
        else if (mode == "suffix")
            ans = oracle.inPostfixLanguage(str);
        else
            ans = oracle.inInfixLanguage(str);
        p.second.push_back(ans);
    }

    for (auto p: extended_prefix) {
        bool ans;
        std::string str = p.first + full_suf;
        if (mode == "prefix")
            ans = oracle.inPrefixLanguage(str);
        else if (mode == "suffix")
            ans = oracle.inPostfixLanguage(str);
        else
            ans = oracle.inInfixLanguage(str);
        p.second.push_back(ans);
    }
}

void ObservationTable::make_consistence_and_closure() {
    char suf = '-';
    std::string pref1, pref2;
    while (!is_consistent(pref1, pref2, suf) || !is_closed()) {
        if (!is_closed())
            make_closure();
        if (suf != '-')
            make_consistence(pref1, pref2, suf);
    }
}

DFA ObservationTable::convert_to_dfa() {
    DFA dfa(alphabet);

    std::unordered_map<std::vector<bool>, int> mp;
    std::unordered_map<std::vector<bool>, std::string> mp1;
    std::set<std::vector<bool>> states;
    int count = 0;

    for (auto p: prefix) {
        if (!states.contains(p.second)) {
            states.insert(p.second);
            mp[p.second] = count;
            mp1[p.second] = p.first;
            if (p.second[0])
                dfa.addFinalState(count++);
        }
    }

    for (auto state: states) {
        std::string pref = mp1[state];
        for (auto c: alphabet) {
            std::string c_str = std::string(1, c);
            if (prefix.contains(pref + c_str)) {
                dfa.addTransition(
                        mp[state],
                        c,
                        mp[prefix[pref + c_str]]
                );
            }
        }
    }

    return dfa;
}

void ObservationTable::add_counterexample(std::string s) {
    for (int i = 0; i < s.size(); i++) {
        std::string sub = s.substr(s.size() - i - 1, s.size());
        if (!prefix.contains(sub) && !extended_prefix.contains(sub)) {
            std::vector<bool> vec;
            for (auto suf: suffix) {
                std::string str = sub + suf;
                bool ans;
                if (mode == "prefix")
                    ans = oracle.inPrefixLanguage(str);
                else if (mode == "suffix")
                    ans = oracle.inPostfixLanguage(str);
                else
                    ans = oracle.inInfixLanguage(str);
                vec.push_back(ans);
            }
            prefix[sub] = vec;
        } else if (!prefix.contains(sub)) {
            prefix[sub] = extended_prefix[sub];
            extended_prefix.erase(sub);
        }
    }

    // how to efficiently add new strings to extended prefix?
    extended_prefix.clear();
    for (auto p: prefix) {
        for (auto c: alphabet) {
            std::string str = p.first + std::string(1, c);
            if (!prefix.contains(str) && !extended_prefix.contains(str)) {
                std::vector<bool> vec;
                for (auto suf: suffix) {
                    std::string str1 = str + suf;
                    bool ans;
                    if (mode == "prefix")
                        ans = oracle.inPrefixLanguage(str1);
                    else if (mode == "suffix")
                        ans = oracle.inPostfixLanguage(str1);
                    else
                        ans = oracle.inInfixLanguage(str1);
                    vec.push_back(ans);
                }
                extended_prefix[str] = vec;
            }
        }
    }
}