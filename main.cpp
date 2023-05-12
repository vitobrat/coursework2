#include <iostream>
#include <sstream>
#include <stack>

using namespace std;

int checkInput(){
    int input;
    try {
        cin >> input;
        if (cin.fail()) {
            throw 1;
        }
    } catch (int exeption) {
        cout << "ERROR!!!";
        exit(0);
    }
    cin.sync();
    cout << "\n";
    return input;
}

struct Node{
    int key;
    int height = 0;
    Node *left = nullptr;
    Node *right = nullptr;
};

int height(Node* node) {
    if (node != nullptr) {
        return node->height;
    } else {
        return 0;
    }
}

int balanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    } else {
        return (height(node->right) - height(node->left));
    }
}

void fixHeight(Node* node) {
    int left = height(node->left);
    int right = height(node->right);
    if (left > right) {
        node->height = left + 1;
    } else {
        node->height = right + 1;
    }
}

Node* rotateLeft(Node* node) {
    Node* p = node->right;
    node->right = p->left;
    p->left = node;
    fixHeight(node);
    fixHeight(p);
    return p;
}

Node* rotateRight(Node* node) {
    Node* p = node->left;
    node->left = p->right;
    p->right = node;
    fixHeight(node);
    fixHeight(p);
    return p;
}

Node* balance(Node* root) {
    fixHeight(root);
    if (balanceFactor(root) == 2) {
        if (balanceFactor(root->right) < 0) {
            root->right = rotateRight(root->right);
        }
        return rotateLeft(root);
    }
    if (balanceFactor(root) == -2) {
        if (balanceFactor(root->left) > 0) {
            root->left = rotateLeft(root->left);
        }
        return rotateRight(root);
    }
    return root;
}
Node* insert(Node* node, int k) {
    if (!node) {
        Node* newNode = new Node;
        newNode->key = k;
        return newNode;
    }
    if (k < node->key) {
        node->left = insert(node->left, k);
    } else if (k > node->key) {
        node->right = insert(node->right, k);
    }
    return balance(node);
}

Node* search(Node *node, int k, stack<Node> stack){
    if(!node){
        return nullptr;
    }else{
        stack.push(*node);
        if(k == node->key){
            return node;
        }
        if(k > node->key){
            return search(node->right, k, stack);
        }else{
            return search(node->left, k, stack);
        }
    }
}

Node* getMax(Node *node, stack<Node> stack){
    while (node->right) {
        stack.push(*node);
        node = node->right;
    }
    Node *outPut = node;
    //надо написать, чтобы максимальный элемент удалялся и у его родителя->right = null
    return node;
}

Node* deleteNode(Node *node, int k){
    stack<Node> stack;
    Node *deleteElement = search(node, k, stack);
    if(deleteElement){
        if(deleteElement->left){
            swap(deleteElement->key, getMax(deleteElement->left, stack)->key);

        }else{

        }
    }else {
        return nullptr;
    }
}

void printTree(Node* root, std::string prefix = "", bool isLeft = true) {
    if (root == nullptr) {
        return;
    }
    printTree(root->right, prefix + (isLeft ? "|   " : "    "), false);
    std::cout << prefix;
    std::cout << (isLeft ? "\\--" : "/--");
    std::cout << root->key << std::endl;
    printTree(root->left, prefix + (isLeft ? "|   " : "    "), true);
}

void function1(Node *&root){
    cout << "Which type of input do you prefer(1 - random, 2 - ourselves): ";
    int inputType = checkInput();
    int size;
    string stringList;
    if(inputType == 1){
        cout << "Input size: \n";
        size = checkInput();
        if (size <= 0){
            cout << "Wrong input! Try again.\n";
            function1(root);
        }
        srand(time(NULL));
        for(int i = 0; i < size; i++){
            root = insert(root, rand() % 199 - 99);
        }
    }else if(inputType == 2){
        cout << "Input list: \n";
        getline(cin, stringList);
        istringstream iss(stringList);
        int num;
        while (iss >> num) {
            root = insert(root, num);
        }
    }else{
        cout << "Wrong input! Try again.\n";
        function1(root);
    }
}

int main() {
    Node *root = nullptr;
    function1(root);
    printTree(root);
    getchar();
    return 0;
}
