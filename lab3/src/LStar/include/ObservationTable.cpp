#include "ObservationTable.h"

bool ObservationTable::check_string(std::string &s) {
    bool ans;
    if (mode == "prefix")
        ans = oracle.inPrefixLanguage(s);
    else if (mode == "suffix")
        ans = oracle.inPostfixLanguage(s);
    else
        ans = oracle.inInfixLanguage(s);

    if (!ans)
        return false;

    if (mode == "prefix") {
        for (int i = 0; i < limit_pump; i++) {
            std::string pumped = s;
            for (int j = 0; j < i; j++) {
                pumped.append(partition[1]);
            }
            pumped.append(partition[2]);
            for (int j = 0; j < i; j++) {
                pumped.append(partition[3]);
            }

            if (!oracle.inPrefixLanguage(pumped))
                return false;
        }
    } else if (mode == "suffix") {
        for (int i = 0; i < limit_pump; i++) {
            std::string pumped = "";
            for (int j = 0; j < i; j++) {
                pumped.append(partition[1]);
            }
            pumped.append(partition[2]);
            for (int j = 0; j < i; j++) {
                pumped.append(partition[3]);
            }
            pumped.append(s);

            if (!oracle.inPostfixLanguage(pumped))
                return false;
        }
    } else {
        for (int i = 0; i < limit_pump; i++) {
            std::string pumped = "";
            for (int j = 0; j < i; j++) {
                pumped.append(partition[1]);
            }
            pumped.append(s);
            for (int j = 0; j < i; j++) {
                pumped.append(partition[3]);
            }

            if (!oracle.inInfixLanguage(pumped))
                return false;
        }
    }

    return true;
}

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
            extended_prefix.erase(p.first);
            extended_rows.erase(p.second);
            for (auto c: alphabet) {
                std::string str = p.first + c;
                std::vector<bool> vec;
                for (auto suf: suffix) {
                    std::string with_suf = str + suf;
                    vec.push_back(check_string(with_suf));
                }
                extended_prefix[str] = vec;
                extended_rows.insert(vec);
            }
            break;
        }
    }
}

bool ObservationTable::is_consistent(std::string &pref1, std::string &pref2, char &suf) {
    for (auto it = prefix.begin(); it != prefix.end(); it++) {
        for (auto it_2 = std::next(it); it_2 != prefix.end(); it_2++) {
            if (std::equal(it->second.begin(), it->second.end(), it_2->second.begin(), it_2->second.end())) {
                for (auto c: alphabet) {
                    std::vector<bool> vec1, vec2;
                    pref1 = it->first;
                    pref2 = it_2->first;
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
    rows.clear();
    extended_rows.clear();

    for (auto &p: prefix) {
        std::string str = p.first + full_suf;
        p.second.push_back(check_string(str));
        rows.insert(p.second);
    }

    for (auto &p: extended_prefix) {
        std::string str = p.first + full_suf;
        p.second.push_back(check_string(str));
        extended_rows.insert(p.second);
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
            else
                count++;
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
            } else if (extended_prefix.contains(pref + c_str)) {
                dfa.addTransition(
                        mp[state],
                        c,
                        mp[extended_prefix[pref + c_str]]
                );
            }
        }
    }

    dfa.setCount(count);

    return dfa;
}

void ObservationTable::add_counterexample(std::string s) {
    for (int i = 0; i < s.size(); i++) {
        std::string sub = s.substr(0, s.size() - i);
        if (prefix.contains(sub))
            continue;
        if (!prefix.contains(sub) && !extended_prefix.contains(sub)) {
            std::vector<bool> vec;
            for (auto suf: suffix) {
                std::string str = sub + suf;
                vec.push_back(check_string(str));
            }
            prefix[sub] = vec;
            rows.insert(vec);
        } else if (!prefix.contains(sub)) {
            prefix[sub] = extended_prefix[sub];
            rows.insert(extended_prefix[sub]);
            extended_rows.erase(extended_prefix[sub]);
            extended_prefix.erase(sub);
        }
//        for (auto c: alphabet) {
//            std::string str = sub + c;
//            if (!extended_prefix.contains(str)) {
//                std::vector<bool> vec;
//                for (auto suf: suffix) {
//                    std::string with_suf = str + suf;
//                    vec.push_back(check_string(str));
//                }
//                extended_prefix[str] = vec;
//                extended_rows.insert(vec);
//            }
//        }
    }

    // more efficiently add new strings to extended prefix?
    extended_prefix.clear();
    extended_rows.clear();
    for (auto p: prefix) {
        for (auto c: alphabet) {
            std::string str = p.first + c;
            if (!prefix.contains(str) && !extended_prefix.contains(str)) {
                std::vector<bool> vec;
                for (auto suf: suffix) {
                    std::string with_suf = str + suf;
                    vec.push_back(check_string(with_suf));
                }
                extended_prefix[str] = vec;
                extended_rows.insert(vec);
            }
        }
    }

    make_consistence_and_closure();
}

void ObservationTable::print_table() {
    printf("%10s", "");
    for (auto suf: suffix) {
        if (suf == "")
            printf("%10s", "eps");
        else
            printf("%10s", suf.c_str());
    }
    std::cout << std::endl;

    for (auto pref: prefix) {
        if (pref.first == "")
            printf("%10s", "eps");
        else
            printf("%10s", pref.first.c_str());
        for (auto el: pref.second) {
            printf("%10d", el ? 1 : 0);
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    for (auto pref: extended_prefix) {
        printf("%10s", pref.first.c_str());
        for (auto el: pref.second) {
            printf("%10d", el ? 1 : 0);
        }
        std::cout << std::endl;
    }
}