#ifndef LAB3_OBSERVATIONTABLE_H
#define LAB3_OBSERVATIONTABLE_H
#include <string>
#include "OracleModule.hpp"
#include <set>
#include <map>
#include <unordered_map>
#include "DFA.hpp"


class ObservationTable {
protected:
    std::vector<std::string> partition;
    int limit_pump;
    std::string mode;
    OracleModule oracle;
    std::set<char> alphabet;
    std::vector<std::string> suffix;
    std::map<std::string, std::vector<bool>> prefix;
    std::unordered_map<std::string, std::vector<bool>> extended_prefix;
    std::set<std::vector<bool>> rows;
    std::set<std::vector<bool>> extended_rows;
    bool is_consistent(std::string&, std::string&, char&);
    bool is_closed();
    void make_closure();
    void make_consistence(std::string&, std::string&, char&);
public:
    bool check_string(std::string&);
    void print_table();
    void make_consistence_and_closure();
    ObservationTable(OracleModule oracle, std::string mode, std::set<char> alphabet,
                     int limit_pump, std::vector<std::string> partition):
                     oracle(oracle), mode(mode), alphabet(alphabet), limit_pump(limit_pump), partition(partition)
    {
        if (mode != "prefix" && mode != "suffix" && mode != "infix" && mode != "antiprefix" && mode != "antisuffix")
            throw std::invalid_argument("Mode must be prefix, suffix or infix");

        std::string eps = "";
        suffix.push_back(eps);
        prefix[eps].push_back(check_string(eps));
        rows.insert(prefix[eps]);

        for (auto c: alphabet) {
            std::string s = std::string(1, c);
            extended_prefix[s].push_back(check_string(s));
            extended_rows.insert(extended_prefix[s]);
        }

        make_consistence_and_closure();
    }

    DFA convert_to_dfa();
    void add_counterexample(std::string);
};

class CounterTable: public ObservationTable {
private:
    std::set<std::string> examples; // C_k
public:
    CounterTable(OracleModule oracle, std::string mode, std::set<char> alphabet,
                 int limit_pump, std::vector<std::string> partition, std::set<std::string> examples):
            ObservationTable(oracle, mode, alphabet, limit_pump, partition), examples(examples) {};

    bool check_string(std::string&)
};


#endif //LAB3_OBSERVATIONTABLE_H
