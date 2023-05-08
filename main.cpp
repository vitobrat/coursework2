#include <iostream>
#include <sstream>

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
    int height;
    Node *left = nullptr;
    Node *right = nullptr;
};

int height(Node* p) {
    if (p != nullptr) {
        return p->height;
    } else {
        return 0;
    }
}

int balance_factor(Node* p) {
    if (p == nullptr) {
        return 0;
    } else {
        return (height(p->right) - height(p->left));
    }
}

void fix_height(Node* p) {
    int h_left = height(p->left);
    int h_right = height(p->right);
    if (h_left > h_right) {
        p->height = h_left + 1;
    } else {
        p->height = h_right + 1;
    }
}

Node* RotateLeft(Node* tree) {
    Node* p = tree->right;
    tree->right = p->left;
    p->left = tree;
    fix_height(tree);
    fix_height(p);
    return p;
}

Node* RotateRight(Node* tree) {
    Node* p = tree->left;
    tree->left = p->right;
    p->right = tree;
    fix_height(tree);
    fix_height(p);
    return p;
}

Node* balance(Node* root) {
    fix_height(root);
    if (balance_factor(root) == 2) {
        if (balance_factor(root->right) < 0) {
            root->right = RotateRight(root->right);
        }
        return RotateLeft(root);
    }
    if (balance_factor(root) == -2) {
        if (balance_factor(root->left) > 0) {
            root->left = RotateLeft(root->left);
        }
        return RotateRight(root);
    }
    return root;
}
Node* insert(Node* p, int k) {
    if (!p) {
        Node* x = new Node;
        x->key = k;
        return x;
    }
    if (k < p->key) {
        p->left = insert(p->left, k);
    } else if (k > p->key) {
        p->right = insert(p->right, k);
    }
    return balance(p);
}

void function1(Node *root){
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
            insert(root, rand() % 199 - 99);
        }
    }else if(inputType == 2){
        cout << "Input list: \n";
        getline(cin, stringList);

    }else{
        cout << "Wrong input! Try again.\n";
        function1(root);
    }

}

int main() {
    Node *root = new Node;
    function1(root);
    return 0;
}
