#include <iostream>
#include <sstream>
#include <stack>
#include <fstream>
#include <chrono>

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
    int height = 1;
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

Node* search(Node *node, int k, stack<Node*> &stack){
    if(!node){
        return nullptr;
    }else{
        if(k == node->key){
            return node;
        }
        stack.push(node);
        if(k > node->key){
            return search(node->right, k, stack);
        }else{
            return search(node->left, k, stack);
        }
    }
}

Node* search(Node *node, int k){
    if(!node){
        return nullptr;
    }else{
        if(k == node->key){
            return node;
        }
        if(k > node->key){
            return search(node->right, k);
        }else{
            return search(node->left, k);
        }
    }
}

Node* getMax(Node *node, stack<Node*> &stack){
    Node *prev = node;
    while (node->right) {
        stack.push(node);
        prev = node;
        node = node->right;
    }
    //надо написать, чтобы максимальный элемент удалялся и у его родителя->right = null
    if(node->left && node != prev){
        prev->right = node->left;
    }else{
        prev->right = nullptr;
    }
    return node;
}

Node* deleteNode(Node *node, int k){
    stack<Node*> stack;
    Node *deleteElement = search(node, k, stack);
    if(deleteElement){
        if(deleteElement->left && deleteElement->right){
            stack.push(deleteElement);
            Node *max = getMax(deleteElement->left, stack);
            if(deleteElement->left == max){
                deleteElement->left = max->left;
            }
            deleteElement->key = max->key;
            delete max;
        }else if(deleteElement->left){
            stack.top()->left = deleteElement->left;
            delete deleteElement;
        }else if(deleteElement->right){
            stack.top()->right = deleteElement->right;
            delete deleteElement;
        }else{
            if(stack.top()->left == deleteElement){
                stack.top()->left = nullptr;
                delete deleteElement;
            }else {
                stack.top()->right = nullptr;
                delete deleteElement;
            }
        }
    }else {
        return node;
    }
    while (!stack.empty()){
        Node *help = stack.top();
       if(stack.size() == 1){
           node = balance(stack.top());
           break;
       }
       stack.pop();
       stack.top()->left == help ? stack.top()->left = balance(help) : stack.top()->right = balance(help);
    }
    return node;
}

void printTree(Node* root, ostream& outputFile, std::string prefix = "", bool isLeft = true) {
    if (root == nullptr) {
        return;
    }
    printTree(root->right, outputFile,  prefix + (isLeft ? "|   " : "    "), false);
    outputFile << prefix;
    outputFile << (isLeft ? "\\--" : "/--");
    outputFile << root->key << endl;
    printTree(root->left,outputFile, prefix + (isLeft ? "|   " : "    "), true);
}

void action(Node *&root, string str, int type, ofstream& output_key, ofstream& output_ans){
    istringstream iss(str); int k;
    while (iss >> k) {
        switch (type) {
            case 0:
            case 1:
                root = insert(root, k);
                break;
            case 2:
                if (!search(root, k)) output_key << k << " is not exist!\n";
                else{
                    root = deleteNode(root, k);
                }
                break;
            case 3:
                if (search(root, k)) output_ans << k << " exist; ";
                else output_ans << k << " not exist; ";
                break;
            default:
                output_key << "Error input!";
        }
        output_key << type <<":\n";
        printTree(root, output_key);
    }
    output_ans << "\n";
    switch (type) {
        case 0:
            output_ans << "Tree after creation:\n";
            break;
        case 1:
            output_ans << "Tree after add elements:\n";
            break;
        case 2:
            output_ans << "Tree after delete elements:\n";
            break;
        case 3:
            output_ans << "Final result:\n";
            break;
    }
    printTree(root, output_ans);
}

void consoleMod(Node *&root){
    cout << "Input list: \n";
    string stringList;
    getline(cin, stringList);
    istringstream iss(stringList);
    int num;
    while (iss >> num) {
        root = insert(root, num);
        printTree(root, cout);
        cout << "\n" << "----------------------------------------------------------------------" << "\n";
    }
    int type = 1;
    while(type) {
        cout << "1)Insert\n"
                "2)Delete\n"
                "3)Search\n"
                "0)exit\n";
        type = checkInput();
        if(type == 0) break;
        int k = checkInput();
        switch (type){
            case 1:
                root = insert(root, k);
                break;
            case 2:
                if (!search(root, k)) cout << k << " is not exist!\n";
                else{
                    root = deleteNode(root, k);
                }
                break;
            case 3:
                if(search(root, k)) cout << k << " exist;";
                else cout << k <<  " not exist;";
                break;
            default:
                cout << "Error input!";
        }
        cout << "\n";
        printTree(root, cout);
    }
}

void fileMod(Node *&root){
    ifstream input_task("C:\\FirstCursProgramm\\C++\\coursework2\\input_task.txt");
    ofstream output_key("C:\\FirstCursProgramm\\C++\\coursework2\\output_key.txt");
    ofstream output_ans("C:\\FirstCursProgramm\\C++\\coursework2\\output_ans.txt");
    if (input_task.is_open() && output_ans.is_open() && output_key.is_open()) {
        string line, creatLine, deleteLine, addLine, searchLine;
        for(int i = 0; i < 5; i++) getline(input_task, line);
        while (getline(input_task, line)) {
            getline(input_task, creatLine);
            getline(input_task, deleteLine);
            getline(input_task, addLine);
            getline(input_task, searchLine);
            auto start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            action(root, creatLine, 0, output_key, output_ans);
            auto end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            output_key << "time spent to creat tree in nanoseconds: " << end - start << "ns\n";
            start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            action(root, deleteLine, 2, output_key, output_ans);
            end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            output_key << "time spent to delete elements tree in nanoseconds: " << end - start << "ns\n";
            start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            action(root, addLine, 1, output_key, output_ans);
            end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            output_key << "time spent to add elements tree in nanoseconds: " << end - start << "ns\n";
            start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            action(root, searchLine, 3, output_key, output_ans);
            end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            output_key << "time spent to search elements tree in nanoseconds: " << end - start << "ns\n";
            output_ans << "----------------------------";
            output_key << "----------------------------";
            root = nullptr;
        }
        input_task.close();
        output_ans.close();
        output_key.close();
    } else {
        std::cout << "Could not open the file!!!" << std::endl;
    }
}

int main() {
    Node *root = nullptr;
    cout << "Which type of input do you prefer(1 - file, 2 - ourselves): ";
    int inputType = checkInput();
    if(inputType == 1){
        fileMod(root);
    }else if(inputType == 2){
        consoleMod(root);
    }else{
        cout << "Wrong input!\n";
        exit(1);
    }
    getchar();
    return 0;
}
