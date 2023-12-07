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
            pumped.reserve(s.size()+ partition[1].size()*i + partition[3].size()*i + partition[2].size());
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
            pumped.reserve(s.size()+ partition[1].size()*i + partition[3].size()*i + partition[2].size());
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
    } else if (mode == "infix") {
        for (int i = 0; i < limit_pump; i++) {
            std::string pumped = "";
            pumped.reserve(s.size()+ partition[1].size()*i + partition[3].size()*i);
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
    } else if (mode == "antiprefix") {
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

            if (oracle.inPostfixLanguage(pumped))
                return false;
        }
    } else {
        for (int i = 0; i < limit_pump; i++) {
            std::string pumped = s;
            pumped.reserve(s.size()+ partition[1].size()*i + partition[3].size()*i + partition[2].size());
            for (int j = 0; j < i; j++) {
                pumped.append(partition[1]);
            }
            pumped.append(partition[2]);
            for (int j = 0; j < i; j++) {
                pumped.append(partition[3]);
            }

            if (oracle.inPrefixLanguage(pumped))
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
    }

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


/*
bool CounterTable::check_string(std::string &s) {
    if (mode == "prefix") {
        for (auto &ex: examples) {
            for (int i = 0; i < limit_pump; i++) {
                std::string to_check = s;
                for (int j = 0; j < i; j++)
                    to_check.append(partition[1]);
                to_check.append(partition[2]);
                for (int j = 0; j < i; j++)
                    to_check.append(partition[3]);
                to_check.append(ex);

                if (oracle.inLanguage(to_check))
                    return false;
            }
        }
    } else {
        for (auto &ex: examples) {
            for (int i = 0; i < limit_pump; i++) {
                std::string to_check = ex;
                for (int j = 0; j < i; j++)
                    to_check.append(partition[1]);
                to_check.append(partition[2]);
                for (int j = 0; j < i; j++)
                    to_check.append(partition[3]);
                to_check.append(s);

                if (oracle.inLanguage(to_check))
                    return false;
            }
        }
    }

    return true;
}
*/

bool CounterTable::check_string(std::string &s) {
    if (mode == "prefix" && !pref.checkString(s))
        return false;
    else if (mode == "suffix" && !suf.checkString(s))
        return false;

    if (mode == "prefix" && full_checked[0].contains(s))
        return true;
    else if (mode == "suffix" && full_checked[1].contains(s))
        return true;

    for (int i = 0; i < 35; i++) {
        if (mode == "prefix") {
            std::string generated = suf.getRandomString();
            for (int pump = 0; pump < limit_pump; pump++) {
                std::string to_check = s;
                for (int j = 0; j < pump; j++)
                    to_check.append(partition[1]);
                to_check.append(partition[2]);
                for (int j = 0; j < pump; j++)
                    to_check.append(partition[3]);
                to_check.append(generated);

                if (!oracle.inLanguage(to_check)) {
                    full_checked[0].insert(s);
                    return true;
                }
            }
        } else {
            std::string generated = pref.getRandomString();
            for (int pump = 0; pump < limit_pump; pump++) {
                std::string to_check = generated;
                for (int j = 0; j < pump; j++)
                    to_check.append(partition[1]);
                to_check.append(partition[2]);
                for (int j = 0; j < pump; j++)
                    to_check.append(partition[3]);
                to_check.append(s);

                if (!oracle.inLanguage(to_check)) {
                    full_checked[1].insert(s);
                    return true;
                }
            }
        }
    }

    return false;
}

bool CounterTable::is_closed() {
    std::set<std::vector<bool>> diff;
    std::set_difference(extended_rows.begin(), extended_rows.end(), rows.begin(), rows.end(),
                        std::inserter(diff, diff.begin()));
    return diff.empty();
}

void CounterTable::make_closure() {
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

bool CounterTable::is_consistent(std::string &pref1, std::string &pref2, char &suf) {
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

void CounterTable::make_consistence(std::string &pref1, std::string &pref2, char &suf) {
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

void CounterTable::make_consistence_and_closure() {
    char suf = '-';
    std::string pref1, pref2;
    while (!is_consistent(pref1, pref2, suf) || !is_closed()) {
        if (!is_closed())
            make_closure();
        if (suf != '-')
            make_consistence(pref1, pref2, suf);
    }
}

DFA CounterTable::convert_to_dfa() {
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

void CounterTable::add_counterexample(std::string s) {
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
    }

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

void CounterTable::print_table() {
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