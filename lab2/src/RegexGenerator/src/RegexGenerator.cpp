
#include <random>
#include <fstream>
#include "RegexGenerator.hpp"

const int EMPTY(-1);
const int EPS(0);
const int WORD(1);
const int OPEN_BRACKET(2);
const int CLOSE_BRACKET(3);
const int ALTERNATIVE(4);
const int SHUFFLE(5);
const int CONCAT(6);
const int KLINI(7);


std::unordered_map<std::string, int> mp = {{"word", WORD},
                                            {"(",    OPEN_BRACKET},
                                            {")",    CLOSE_BRACKET},
                                            {"|",    ALTERNATIVE},
                                            {"*",    KLINI},
                                            {"·",    CONCAT},
                                            {"#",    SHUFFLE},
                                            {"∅",    EMPTY},
                                            {"ϵ",    EPS}};

RegexGenerator::RegexGenerator(int alphabet_len, int star_height, int max_len) : alphabet_len(alphabet_len),
                                                                                 star_height(star_height),
                                                                                 max_len(max_len),
                                                                                 len(0){
}

bool randomBool() {
    auto gen = rand() % 2;
    return gen;
}

std::string random_elem(std::vector<std::string> &v) {
    if(v.size() == 1){
        return v[0];
    }
    auto gen = rand() % (v.size()-1);
    return v[gen];
}
int randint(int low, int high){
    if(high == low)
        return low;
    auto gen = low +rand() % (high-low) ;
    return gen;
}
std::vector<std::string> gen_alphabet(int n) {
    std::vector<std::string> ans(n);
    for (int i = 0; i < n; i++) {
        ans[i] = std::string(1, char('a' +char(i)));
    }
    return ans;
}

Node* RegexGenerator::inner_gen(Node *root, int node_num,int max_star) {
    if (node_num > 2) {
        if (randomBool() || max_star == 0) {
            root->data.second = random_elem(binary);
            root->data.first = mp[root->data.second];
            root->left = new Node;
            root->right = new Node;
            int l = randint(1, node_num-2);
            root->left = inner_gen(root->left, l, max_star);
            root->right = inner_gen(root->right, node_num-1-l, max_star);
        }else{
            root->data.second = random_elem(unary);
            root->data.first = mp[root->data.second];
            root->left = new Node;
            root->left = inner_gen(root->left, node_num-1, max_star-1);
        }
    }else{
        if(node_num == 1){
            root->data.first = WORD;
            root->data.second = random_elem(alphabet);
            return root;
        }
        if(node_num == 2){
            root->data.second = random_elem(unary);
            root->data.first = mp[root->data.second];
            root->left = new Node;
            root->left = inner_gen(root->left, node_num-1, max_star-1);
        }
    }
    return root;
}

std::vector<Regex> RegexGenerator::gen(int number) {
    std::vector<Regex> ans;
    alphabet = gen_alphabet(alphabet_len);
    for (int i = 0; i < number; i++){
        Node *root = new Node;
        root = inner_gen(root, max_len, star_height);
        ans.emplace_back(root);
    }
    return ans;
}

RegexGenerator::RegexGenerator(std::string &path) {
    std::string cnt;
    std::ifstream file(path);
    getline(file,cnt, ' ');
    alphabet_len = std::stoi(cnt);
    getline(file,cnt, ' ');
    star_height = std::stoi(cnt);
    getline(file,cnt, ' ');
    max_len = std::stoi(cnt);
    getline(file,cnt, ' ');
    len = std::stoi(cnt);
    file.close();
}

std::vector<Regex> RegexGenerator::gen() {
    return gen(len);
}


