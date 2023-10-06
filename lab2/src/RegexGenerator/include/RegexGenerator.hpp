#ifndef LAB2_REGEXGENERATOR_HPP
#define LAB2_REGEXGENERATOR_HPP
#include <iostream>
#include <vector>
#include "Node.hpp"
#include "Derivative.hpp"

class RegexGenerator{
public:
    RegexGenerator(int alphabet_len, int star_height, int max_len);
    explicit RegexGenerator(std::string& path);
    std::vector<Regex> gen(int number);
    std::vector<Regex> gen();
private:
    int alphabet_len;
    int star_height;
    int max_len;
    int len;
    std::vector<std::string> binary = {"#", "|", "·"};
    std::vector<std::string> unary = {"*"};
    std::vector<std::string> alphabet;
    Node* inner_gen(Node* root, int num, int max_star);
};
#endif //LAB2_REGEXGENERATOR_HPP
