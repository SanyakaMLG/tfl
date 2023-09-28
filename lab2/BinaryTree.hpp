#ifndef LAB2_BINARYTREE_HPP
#define LAB2_BINARYTREE_HPP

#include <iostream>
#include <stack>

#endif //LAB2_BINARYTREE_HPP
template<typename T>
struct Node {
    explicit Node(const T &data_) : data(data_), left(nullptr), right(nullptr) {
    }

    Node(const T &data_, Node *l, Node *r) : data(data_), left(l), right(r) {
    }

    Node() = default;

    T data;
    Node *left;
    Node *right;
};
template<typename T>
Node<T>* cloneBinaryTree(Node<T>* root)
{
    if (root == nullptr) {
        return nullptr;
    }

    Node<T>* root_copy = new Node(root->data);

    root_copy->left = cloneBinaryTree(root->left);
    root_copy->right = cloneBinaryTree(root->right);

    return root_copy;
}
template<typename T>
bool equalTrees(Node<T>* l, Node<T>* r){
    if(!l && !r){
        return true;
    }
    if((l && !r) ||  !l){
        return false;
    }
    return (l->data == r->data) && equalTrees(l->right, r->right) && equalTrees(l->left, r->left);
}
template<typename T, typename Comparator=std::less<T>>
class BinaryTree {
    struct Node {
        explicit Node(const T &data_) : data(data_), left(nullptr), right(nullptr) {
        }

        T data;
        Node *left;
        Node *right;
    };

public:
    BinaryTree() : root(nullptr) {
    }

    explicit BinaryTree(Comparator comp) : root(nullptr), comp(comp) {
    }

    ~BinaryTree() {
        std::stack<Node *> stack;
        if (root)
            stack.push(root);
        while (!stack.empty()) {
            Node *node = stack.top();
            stack.pop();
            if (node->right)
                stack.push(node->right);
            if (node->left)
                stack.push(node->left);
            delete node;
        }
    }

private:
    Node *root;
    Comparator comp;
};