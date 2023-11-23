#ifndef LAB3_OBSERVATIONTABLE_H
#define LAB3_OBSERVATIONTABLE_H
#include <string>
#include "OracleModule.hpp"
#include <set>
#include <unordered_map>
#include "DFA.hpp"


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
    bool is_consistent(std::string&, std::string&, char&);
    bool is_closed();
    void make_closure();
    void make_consistence(std::string&, std::string&, char&);
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

        make_consistence_and_closure();
    }
    void make_consistence_and_closure();
    DFA convert_to_dfa();
    void add_counterexample(std::string);
};


#endif //LAB3_OBSERVATIONTABLE_H
