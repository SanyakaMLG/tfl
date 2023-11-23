#ifndef LAB3_OBSERVATIONTABLE_H
#define LAB3_OBSERVATIONTABLE_H
#include <string>
#include "OracleModule.hpp"
#include <set>
#include <unordered_map>


class ObservationTable {
private:
    std::string mode;
    OracleModule oracle;
    std::set<char> alphabet;
    std::vector<std::string> suffix;
    std::unordered_map<std::string, std::vector<bool>> prefix;
    std::unordered_map<std::string, std::vector<bool>> extended_prefix;
    std::set<std::vector<bool>> rows;
    std::set<std::vector<bool>> extended_rows;
public:
    ObservationTable(OracleModule oracle, std::string mode, std::set<char> alphabet): oracle(oracle), mode(mode), alphabet(alphabet) {
        if (mode != "prefix" && mode != "suffix" && mode != "infix")
            throw std::invalid_argument("Mode must be prefix, suffix or infix");

        std::string eps = "";
        suffix.push_back(eps);
        bool ans;
        if (mode == "prefix")
            ans = oracle.inPrefixLanguage(eps);
        else if (mode == "suffix")
            ans = oracle.inPostfixLanguage(eps);
        else
            ans = oracle.inInfixLanguage(eps);
        prefix[""].push_back(ans);
        rows.insert(prefix[""]);

        for (auto c: alphabet) {
            std::string s = std::string(1, c);
            if (mode == "prefix")
                ans = oracle.inPrefixLanguage(s);
            else if (mode == "suffix")
                ans = oracle.inPostfixLanguage(s);
            else
                ans = oracle.inInfixLanguage(s);
            prefix[s].push_back(ans);
            extended_rows.insert(prefix[s]);
        }

        char suf = '-';
        std::string pref1, pref2;
        while (!is_consistent(pref1, pref2, suf) || !is_closed()) {
            if (!is_closed())
                make_closure();
            if (suf != '-')
                make_consistence(pref1, pref2, suf);
        }
    }
    bool is_consistent(std::string&, std::string&, char&);
    bool is_closed();
    void make_closure();
    void make_consistence(std::string&, std::string&, char&);
};


#endif //LAB3_OBSERVATIONTABLE_H
