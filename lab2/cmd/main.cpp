#include "BrzozowskiAutomaton.hpp"
#include "DFA.hpp"
#include <regex>
#include <fstream>
#include "RegexGenerator.hpp"
#include "ProgramOptions.hpp"
void test(std::string& s){
    std::regex regexp("\\(eps\\)");
    BrzozowskiAutomaton automata = BrzozowskiAutomaton(s);
    DFA dfa = DFA(s);
    auto outRegex = automata.convertToRegex();
    outRegex = std::regex_replace(outRegex, regexp, "");
    std::cout<<outRegex<<"\n";
    regexp = std::regex(outRegex);
    std::string randStr;
    for (int i = 0; i < 1000; i++) {
        randStr = dfa.getRandomString();
        std::cout<<"testing "<< randStr<<" ";
        if (std::regex_match(randStr, regexp)) {
            std::cout << "success "<<"\n";
        } else {
            std::cout << "incorrect "<<randStr<<"\n";
        }
    }
}
int main(int argc, char *argv[]) {
    ProgramOptions::parse(argc, argv);
    std::string_view stringView = ProgramOptions::str();
    if( ProgramOptions::need_file()) {
        std::ifstream input_file(stringView.data(), std::ios::in);
        std::string line;
        while (getline(input_file, line)){
            test(line);
        }
    }else if(ProgramOptions::use_generator()){
        std::string arg =  stringView.data();
        RegexGenerator r(arg);
        for(auto reg:r.gen()){
            std::string a = reg.get();
            test(a);
        }
    }else{
        std::string r;
        if(!stringView.empty())
             r = stringView.data();
        else
            r = "";
        test(r);
    }
    return 0;
}