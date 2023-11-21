#ifndef LAB2_DERIVATIVE_HPP
#define LAB2_DERIVATIVE_HPP
#include <iostream>
#include <regex>
#include <string>
#include <set>
#include "Node.hpp"
class Regex{
public:
    Regex();

    explicit Regex(std::string s);
    explicit Regex(Node * root);
    Regex(std::string s, Node* root);
    Regex der(char let);
    std::string get();
    Node* getTree();
private:
    std::string reg;
    Node* root;
};


Node* cloneBinaryTree(Node* root);

bool containsEPS(Node* root);

bool equalTrees(Node* l, Node* r);

#endif //LAB2_DERIVATIVE_HPP
