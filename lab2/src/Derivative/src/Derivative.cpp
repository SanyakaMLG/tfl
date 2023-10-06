#include "Derivative.hpp"

#include <utility>

#define DEBUG  0
const int EMPTY(-1);
const int EPS(0);
const int WORD(1);
const int OPEN_BRACKET(2);
const int CLOSE_BRACKET(3);
const int ALTERNATIVE(4);
const int SHUFFLE(5);
const int CONCAT(6);
const int KLINI(7);


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


std::string buildShuffle(std::pair<std::set<std::string>, std::vector<std::string>> pair);

void printBT(const std::string &prefix, const Node *node, bool isLeft);

void printBT(const Node *node) {
    printBT("", node, false);
}

int count_slash(const std::string &expr, int index) {
    int ans = 0;
    while (index >= 0 && expr[index] == '\\') {
        ans++;
        index--;
    }
    return ans;
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
            if (!ans.empty() && ans.back().first == WORD && ans.back().second.length() > 1 && str[i] == '*') {
                std::string word = ans.back().second;
                std::string newWord = word.substr(0, word.length() - 1);
                std::string let(1, word.back());
                ans.pop_back();
                ans.emplace_back(WORD, newWord);
                ans.emplace_back(CONCAT, reverse[CONCAT]);
                ans.emplace_back(WORD, let);
            }
            if (i > 0 && str[i] == '(' && (isalpha(str[i - 1]) || str[i - 1] == ')') && ans.back().first != CONCAT) {
                ans.emplace_back(map["·"], "·");
            }
            std::string s(1, str[i]);
            ans.emplace_back(map[s], s);
            if (i + 1 < str.length() && (str[i] == ')' || str[i] == '*') &&
                (isalpha(str[i + 1]) || str[i + 1] == '(') &&
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

Node *buildTree(const std::vector<std::pair<int, std::string>> &lexems) {
    if(lexems.empty()){
        return new Node(std::pair(EPS, reverse[EPS]));
    }
    std::stack<Node *> nodes;
    for (const auto &lex: lexems) {
        if (lex.first == WORD) {
            auto *node = new Node(lex);
            nodes.push(node);
        }
        if (lex.first == CONCAT || lex.first == SHUFFLE || lex.first == ALTERNATIVE) {
            auto *r = nodes.top();
            nodes.pop();
            auto *l = nodes.top();
            nodes.pop();
            auto *node = new Node(lex, l, r);
            nodes.push(node);
        }
        if (lex.first == KLINI) {
            auto *l = nodes.top();
            nodes.pop();
            auto *node = new Node(lex, l, nullptr);
            nodes.push(node);
        }
    }
    return nodes.top();
}

bool containsEPS(Node *root) {
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

Node *derivative(Node *root, char var) {
    //printBT(root);
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
        Node *l = cloneBinaryTree(root->left);
        Node *r = cloneBinaryTree(root->right);
        root->left = new Node(std::pair(CONCAT, reverse[CONCAT]),
                              derivative(root->left, var), r);
        //printBT(root);
        if (containsEPS(l)) {
            root->right = derivative(root->right, var);
        } else {
            root->right = new Node(std::pair(EMPTY, reverse[EMPTY]));
        }
        //printBT(root);
    } else if (root->data.first == KLINI) {
        while(root->left->data.first == KLINI){
            root = root->left;
        }
        auto r = cloneBinaryTree(root);
        root->data.first = CONCAT;
        root->data.second = reverse[CONCAT];
        root->right = r;
        root->left = derivative(root->left, var);
    } else if (root->data.first == SHUFFLE) {
        root->data.first = ALTERNATIVE;
        root->data.second = reverse[ALTERNATIVE];
        auto l = root->left;
        auto r = root->right;
        auto l_copy = cloneBinaryTree(l);
        auto r_copy = cloneBinaryTree(r);
        root->left = new Node(std::pair(SHUFFLE, reverse[SHUFFLE]));
        root->right = new Node(std::pair(SHUFFLE, reverse[SHUFFLE]));
        root->left->left = derivative(l, var);
        root->left->right = r_copy;
        root->right->left = l_copy;
        root->right->right = derivative(r, var);
    }
    return root;
}

Node *ACI(Node *root) {
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
                root = new Node(std::pair(EMPTY, reverse[EMPTY]));
            } else {
                root->left = ACI(root->left);
                root->right = ACI(root->right);
            }
            break;
        case KLINI:
            if (root->left->data.first == EPS || root->left->data.first == EMPTY) {
                int t = root->left->data.first;
                delete root->left;
                root = new Node(std::pair(t, reverse[t]));
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
                root = new Node(std::pair(EMPTY, reverse[EMPTY]));
            } else {
                root->left = ACI(root->left);
                root->right = ACI(root->right);
            }
            break;
    }
    return root;
}

bool hasEmpty(Node *root) {
    if (root->data.first == WORD || root->data.first == EPS) {
        return false;
    }
    if (root->data.first == EMPTY) {
        return true;
    }
    if (root->data.first == KLINI) {
        return hasEmpty(root->left);
    }
    return hasEmpty(root->left) || hasEmpty(root->right);
}

bool hasReduntantEps(Node *root) {
    if (root->data.first == WORD) {
        return false;
    }
    if (root->data.first == KLINI) {
        return hasReduntantEps(root->left);
    }
    if (root->data.first == SHUFFLE || root->data.first == CONCAT) {
        if (root->left->data.first == EPS || root->right->data.first == EPS) {
            return true;
        }
    }
    if (root->data.first == ALTERNATIVE) {
        if (root->left->data.first == EPS && root->right->data.first == EPS) {
            return false;
        }
        if (root->left->data.first == EPS)
            return hasReduntantEps(root->right);
        if (root->right->data.first == EPS)
            return hasReduntantEps(root->left);
    }
    return hasReduntantEps(root->left) || hasReduntantEps(root->right);

}


std::string buildString(const std::set<std::string> &set, int type) {
    std::string ans = "(";
    for (const auto &elem: set) {
        ans += elem;
        ans += reverse[type];
    }
    ans = ans.substr(0, ans.length() - 1);
    return ans + ")";
}

std::string buildKlini(const std::set<std::string> &set) {
    return "(" + *set.begin() + ")*";
}

std::pair<std::set<std::string>, std::vector<std::string>>
innerSortRegex(Node *cur, Node *parent) {
    switch (cur->data.first) {
        case WORD: {
            return {std::set<std::string>{cur->data.second}, {}};
        }
        case EPS: {
            return {std::set<std::string>{cur->data.second}, {}};
        }
        case EMPTY: {
            return {std::set<std::string>{cur->data.second}, {}};
        }
        case ALTERNATIVE: {
            auto l = innerSortRegex(cur->left, cur);
            auto r = innerSortRegex(cur->right, cur);
            l.second = {};
            r.second = {};
            l.first.merge(r.first);
            if (parent && parent->data.first == ALTERNATIVE) {
                return l;
            } else {
                return {std::set<std::string>{buildString(l.first, ALTERNATIVE)}, {}};
            }
        }
        case SHUFFLE: {
            auto l = innerSortRegex(cur->left, cur);
            auto r = innerSortRegex(cur->right, cur);
            l.first.merge(r.first);
            l.second.insert(l.second.end(), r.first.begin(), r.first.end());
            if (parent && parent->data.first == SHUFFLE) {
                return l;
            } else {
                return {std::set<std::string>{buildShuffle(l)}, {}};
            }
        }
        case CONCAT: {
            auto l = innerSortRegex(cur->left, cur);
            auto r = innerSortRegex(cur->right, cur);
            std::string ls = *l.first.begin();
            std::string rs = *r.first.begin();
            return {std::set<std::string>{ls + rs}, {}};
        }
        case KLINI: {
            auto l = innerSortRegex(cur->left, cur);
            return {std::set<std::string>{buildKlini(l.first)}, {}};
        }
        default:
            return {std::set<std::string>{""}, {}};
    }
}

std::string buildShuffle(std::pair<std::set<std::string>, std::vector<std::string>> pair) {
    std::vector<std::string> vec = pair.second;
    vec.insert(vec.end(), pair.first.begin(), pair.first.end());
    std::erase_if(vec, [](const std::string &s) { return s == reverse[EPS]; });
    if (vec.empty()) {
        vec.push_back(reverse[EPS]);
    } else
        std::sort(vec.begin(), vec.end());
    if (vec.size() == 1) {
        return vec[0];
    }
    std::string ans = "(";
    for (const auto &elem: vec) {
        ans += elem;
        ans += "#";
    }
    ans = ans.substr(0, ans.length() - 1);
    return ans + ")";
}

std::string sortRegex(Node *root) {
    return *innerSortRegex(root, nullptr).first.begin();
}

void printBT(const std::string &prefix, const Node *node, bool isLeft) {
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


Regex Regex::der(char let) {
    Regex ans;
    ans.root = cloneBinaryTree(root);
    //printBT(root);
    auto t = derivative(ans.root, let);
    while (hasEmpty(t) && t->data.first != EMPTY) {
        //printBT(t);
        t = ACI(t);
    }
    while (t->data.first != EPS && t->data.first != EMPTY && hasReduntantEps(t)) {
        t = ACI(t);
    }
#if DEBUG == 1
    printBT(t);
#endif
    std::string r = sortRegex(t);
    ans.root = t;
    ans.reg = r;
    return ans;

}

Regex::Regex(std::string s) : reg(s) {
    root = buildTree(infixToPostfix(get_lexems(reg)));
    reg = sortRegex(root);
}

Regex::Regex(Node *root) : root(root) {
    reg = sortRegex(root);
}

Regex::Regex(std::string s, Node *root) : root(root), reg(s) {

}

std::string Regex::get() {
    return reg;
}

Node *Regex::getTree() {
    return root;
}

Regex::Regex() {

}

