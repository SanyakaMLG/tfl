#ifndef LAB2_NODE_HPP
#define LAB2_NODE_HPP

#include <iostream>
#include <stack>


struct Node {
    explicit Node(const std::pair<int, std::string> &data_);
    Node(const std::pair<int, std::string> &data_, Node *l, Node *r);
    Node() = default;
    std::pair<int, std::string> data;
    Node *left;
    Node *right;
};

#endif