#ifndef LAB2_NODE_CPP
#define LAB2_NODE_CPP

#include "Node.hpp"


Node::Node(const std::pair<int, std::string> &data_) : data(data_), left(nullptr), right(nullptr) {
}

Node::Node(const std::pair<int, std::string> &data_, Node *l, Node *r) : data(data_), left(l), right(r) {
}




Node* cloneBinaryTree(Node* root)
{
    if (root == nullptr) {
        return nullptr;
    }

    Node* root_copy = new Node(root->data);

    root_copy->left = cloneBinaryTree(root->left);
    root_copy->right = cloneBinaryTree(root->right);

    return root_copy;
}

bool equalTrees(Node* l, Node* r){
    if(!l && !r){
        return true;
    }
    if((l && !r) ||  !l){
        return false;
    }
    return (l->data == r->data) && equalTrees(l->right, r->right) && equalTrees(l->left, r->left);
}

#endif