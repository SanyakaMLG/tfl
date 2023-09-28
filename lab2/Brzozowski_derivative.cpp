#include <iostream>
#include <regex>
#include <string>
#include "BinaryTree.hpp"

const int EMPTY(-1);
const int EPS(0);
const int WORD(1);
const int OPEN_BRACKET(2);
const int CLOSE_BRACKET(3);
const int ALTERNATIVE(4);
const int SHUFFLE(5);
const int CONCAT(6);
const int KLINI(7);

int count_slash(const std::string &expr, int index) {
    int ans = 0;
    while (index >= 0 && expr[index] == '\\') {
        ans++;
        index--;
    }
    return ans;
}


std::unordered_map<std::string, int> map = {{"word", WORD},
                                            {"(",    OPEN_BRACKET},
                                            {")",    CLOSE_BRACKET},
                                            {"|",    ALTERNATIVE},
                                            {"*",    KLINI},
                                            {"·",    CONCAT},
                                            {"#",    SHUFFLE},
                                            {"∅",    EMPTY},
                                            {"ϵ",    EPS}};
std::unordered_map<int, std::string> reverse = {{OPEN_BRACKET,  "("},
                                                {CLOSE_BRACKET, ")"},
                                                {ALTERNATIVE,   "|"},
                                                {KLINI,         "*"},
                                                {CONCAT,        "·"},
                                                {EPS,           "ϵ"},
                                                {EMPTY,         "∅"},
                                                {SHUFFLE,       "#"}};

void printBT(const std::string &prefix, const Node<std::pair<int, std::string>> *node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──");

        // print the value of the node
        std::cout << node->data.second << std::endl;

        // enter the next tree level - left and right branch
        printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
        printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void printBT(const Node<std::pair<int, std::string>> *node) {
    printBT("", node, false);
}


std::vector<std::pair<int, std::string>> get_lexems(std::string &str) {
    std::vector<std::pair<int, std::string>> ans;
    std::string acc;
    for (int i = 0; i < str.length(); i++) {
        while (i < str.length() &&
               !(str[i] == '(' || str[i] == ')' || str[i] == '|' || str[i] == '*' || str[i] == '#') ||
               count_slash(str, i) % 2 != 0) {
            acc += str[i++];
        }
        if (acc.length() > 0) {
            ans.emplace_back(map["word"], acc);
            acc = "";
        }
        if ((str[i] == '(' || str[i] == ')' || str[i] == '|' || str[i] == '*' || str[i] == '#') &&
            count_slash(str, i) % 2 == 0) {
            if (i > 0 && str[i] == '(' && (isalpha(str[i - 1]) || str[i - 1] == ')') && ans.back().first != CONCAT) {
                ans.emplace_back(map["·"], "·");
            }
            std::string s(1, str[i]);
            ans.emplace_back(map[s], s);
            if (i + 1 < str.length() && str[i] == ')' && (isalpha(str[i + 1]) || str[i + 1] == '(') &&
                ans.back().first != CONCAT) {
                ans.emplace_back(map["·"], "·");
            }
        }
    }
    return ans;
}

std::vector<std::pair<int, std::string>> infixToPostfix(const std::vector<std::pair<int, std::string>> &lexems) {
    std::vector<std::pair<int, std::string>> ans;
    std::stack<int> stack;
    for (const auto &lex: lexems) {
        switch (lex.first) {
            case WORD:
                ans.push_back(lex);
                break;
            case OPEN_BRACKET:
                stack.push(OPEN_BRACKET);
                break;
            case CLOSE_BRACKET:
                while (stack.top() != OPEN_BRACKET) {
                    ans.emplace_back(stack.top(), reverse[stack.top()]);
                    stack.pop();
                }
                stack.pop();
                break;
            case KLINI:
                if (stack.empty() || stack.top() == OPEN_BRACKET || stack.top() < KLINI) {
                    stack.push(KLINI);
                } else {
                    while (!stack.empty() && stack.top() >= KLINI && stack.top() != OPEN_BRACKET) {
                        ans.emplace_back(stack.top(), reverse[stack.top()]);
                        stack.pop();
                    }
                    stack.push(KLINI);
                }
                break;
            case CONCAT:
                if (stack.empty() || stack.top() == OPEN_BRACKET || stack.top() < CONCAT) {
                    stack.push(CONCAT);
                } else {
                    while (!stack.empty() && stack.top() >= CONCAT && stack.top() != OPEN_BRACKET) {
                        ans.emplace_back(stack.top(), reverse[stack.top()]);
                        stack.pop();
                    }
                    stack.push(CONCAT);
                }
                break;
            case SHUFFLE:
                if (stack.empty() || stack.top() == OPEN_BRACKET || stack.top() < SHUFFLE) {
                    stack.push(SHUFFLE);
                } else {
                    while (!stack.empty() && stack.top() >= SHUFFLE && stack.top() != OPEN_BRACKET) {
                        ans.emplace_back(stack.top(), reverse[stack.top()]);
                        stack.pop();
                    }
                    stack.push(SHUFFLE);
                }
                break;
            case ALTERNATIVE:
                if (stack.empty() || stack.top() == OPEN_BRACKET || stack.top() < ALTERNATIVE) {
                    stack.push(ALTERNATIVE);
                } else {
                    while (!stack.empty() && stack.top() >= ALTERNATIVE && stack.top() != OPEN_BRACKET) {
                        ans.emplace_back(stack.top(), reverse[stack.top()]);
                        stack.pop();
                    }
                    stack.push(ALTERNATIVE);
                }
                break;
        }
    }
    while (!stack.empty()) {
        ans.emplace_back(stack.top(), reverse[stack.top()]);
        stack.pop();
    }
    return ans;
}

Node<std::pair<int, std::string >> *buildTree(const std::vector<std::pair<int, std::string>> &lexems) {
    std::stack<Node<std::pair<int, std::string >> *> nodes;
    for (auto lex: lexems) {
        if (lex.first == WORD) {
            auto *node = new Node<std::pair<int, std::string>>(lex);
            nodes.push(node);
        }
        if (lex.first == CONCAT || lex.first == SHUFFLE || lex.first == ALTERNATIVE) {
            auto *r = nodes.top();
            nodes.pop();
            auto *l = nodes.top();
            nodes.pop();
            auto *node = new Node<std::pair<int, std::string>>(lex, l, r);
            nodes.push(node);
        }
        if (lex.first == KLINI) {
            auto *l = nodes.top();
            nodes.pop();
            auto *node = new Node<std::pair<int, std::string >>(lex, l, nullptr);
            nodes.push(node);
        }
    }
    return nodes.top();
}

bool containsEPS(Node<std::pair<int, std::string >> *root) {
    if (!root) {
        return false;
    }
    if (root->data.first == EPS || root->data.first == KLINI) {
        return true;
    }
    if (root->data.first == CONCAT) {
        return containsEPS(root->left) && containsEPS(root->right);
    }
    if (root->data.first == ALTERNATIVE) {
        return containsEPS(root->left) || containsEPS(root->right);
    }
    if (root->data.first == SHUFFLE) {
        return containsEPS(root->left) && containsEPS(root->right);
    }
    return false;
}

Node<std::pair<int, std::string >> *derivative(Node<std::pair<int, std::string >> *root, char var) {
    if (root->data.first == WORD) {
        if (root->data.second[0] == var) {
            if (root->data.second.length() > 1) {
                std::pair<int, std::string> d(WORD, root->data.second.substr(1, root->data.second.length() - 1));
                root->data = d;
            } else {
                std::pair<int, std::string> d(EPS, reverse[EPS]);
                root->data = d;
            }
        } else {
            std::pair<int, std::string> d(EMPTY, reverse[EMPTY]);
            //auto n = new Node<std::pair<int, std::string >>(d);
            root->data = d;
        }
    } else if (root->data.first == EMPTY || root->data.first == EPS) {
        std::pair<int, std::string> d(EMPTY, "");
        //auto n = new Node<std::pair<int, std::string >>(d);
        root->data = d;
    } else if (root->data.first == ALTERNATIVE) {
        root->left = derivative(root->left, var);
        root->right = derivative(root->right, var);
    } else if (root->data.first == CONCAT) {
        root->data.first = ALTERNATIVE;
        root->data.second = reverse[ALTERNATIVE];
        Node<std::pair<int, std::string >> *cur = cloneBinaryTree(root->left);
        root->left = new Node<std::pair<int, std::string >>(std::pair(CONCAT, reverse[CONCAT]),
                                                            derivative(root->left, var), root->right);
        std::cout << std::endl;
        if (containsEPS(cur)) {
            root->right = derivative(root->right, var);
        } else {
            root->right = new Node<std::pair<int, std::string >>(std::pair(EMPTY, reverse[EMPTY]));
        }
    } else if (root->data.first == KLINI) {
        root->data.first = CONCAT;
        root->data.second = reverse[CONCAT];
        root->right = root;
        root->left = derivative(root->left, var);
    } else if (root->data.first == SHUFFLE) {
        root->data.first = ALTERNATIVE;
        root->data.second = reverse[ALTERNATIVE];
        auto l = root->left;
        auto r = root->right;
        auto l_copy = cloneBinaryTree(l);
        auto r_copy = cloneBinaryTree(r);
        root->left = new Node<std::pair<int, std::string >>(std::pair(SHUFFLE, reverse[SHUFFLE]));
        root->right = new Node<std::pair<int, std::string >>(std::pair(SHUFFLE, reverse[SHUFFLE]));
        root->left->left = derivative(l, var);
        root->left->right = r_copy;
        root->right->left = l_copy;
        root->right->right = derivative(r, var);
    }
    return root;
}

Node<std::pair<int, std::string >> *ACI(Node<std::pair<int, std::string >> *root) {
    switch (root->data.first) {
        case ALTERNATIVE:
            if (root->left->data.first == EMPTY) {
                delete root->left;
                root = root->right;
                root = ACI(root);
            } else if (root->right->data.first == EMPTY) {
                delete root->right;
                root = root->left;
                root = ACI(root);
            } else if (root->left->data == root->right->data &&
                       (root->left->data.first == WORD || root->left->data.first == EPS)) {
                delete root->left;
                root = root->right;
            } else if (equalTrees(root->left, root->right)) {
                delete root->left;
                root = root->right;
                root = ACI(root);
            } else {
                root->left = ACI(root->left);
                root->right = ACI(root->right);
            }
            break;
        case CONCAT:
            if (root->left->data.first == EPS) {
                delete root->left;
                root = root->right;
                root = ACI(root);
            } else if (root->right->data.first == EPS) {
                delete root->right;
                root = root->left;
                root = ACI(root);
            } else if (root->left->data.first == EMPTY || root->right->data.first == EMPTY) {
                delete root->left;
                delete root->right;
                root = new Node<std::pair<int, std::string>>(std::pair(EMPTY, reverse[EMPTY]));
            } else {
                root->left = ACI(root->left);
                root->right = ACI(root->right);
            }
            break;
        case KLINI:
            if (root->left->data.first == EPS || root->left->data.first == EMPTY) {
                int t = root->left->data.first;
                delete root->left;
                root = new Node<std::pair<int, std::string>>(std::pair(t, reverse[t]));
            } else {
                root->left = ACI(root->left);
            }
            break;
        case SHUFFLE:
            if (root->left->data.first == EPS) {
                delete root->left;
                root = root->right;
                root = ACI(root);
            } else if (root->right->data.first == EPS) {
                delete root->right;
                root = root->left;
                root = ACI(root);
            } else if (root->left->data.first == EMPTY || root->right->data.first == EMPTY) {
                delete root->left;
                delete root->right;
                root = new Node<std::pair<int, std::string>>(std::pair(EMPTY, reverse[EMPTY]));
            } else {
                root->left = ACI(root->left);
                root->right = ACI(root->right);
            }
            break;
    }
    return root;
}

int main() {
    std::string le = "(ab#ac)((ab#c)*)|k";
    std::cout << le << std::endl;
    auto lex = get_lexems(le);
    auto ans = infixToPostfix(lex);
    auto *t = buildTree(ans);
    printBT(t);
    auto *d = derivative(t, 'a');
    printBT(d);
    auto *optim = ACI(d);
    printBT(optim);
    return 0;
}
