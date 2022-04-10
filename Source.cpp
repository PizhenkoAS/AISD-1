#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct Node{
    int key;
    string value;

    Node* left;
    Node* right;

    Node(int k, string v){
        key = k; value = v;
        left = right = nullptr;
    }
};

Node* find(Node* root, int key){
    if (!root){
        return nullptr;
    }
    if (root->key == key){
        return root;
    }
    if (key > root->key){
        return find(root->right, key);
    }
    return find(root->left, key);
}

bool insert(Node*& root, int key, string value){
    if (!root){
        root = new Node(key, value);
        return true;
    }
    if (root->key == key){
        return false;
    }
    if (key > root->key){
        return insert(root->right, key, value);
    }
    return insert(root->left, key, value);
}

Node* min(Node* root){
    if (!root){
        return nullptr;
    }
    Node* temp = root;
    while(temp->left){
        temp = temp->left;
    }
    return temp;
}

bool erase(Node*& root, int key){
    if (!root){
        return false;
    }
    if (root->key == key){
        if ( (!root->left) && (!root->right) ) {
            delete root;
            return true;
        }
        if ( (!root->left) && root->right ){
            root = root->right;
            return true;
        }
        if ( root->left && (!root->right) ){
            root = root->left;
            return true;
        }
        if ( root->left && root->right){
            Node* right_min = min(root->right);
            root->key = right_min->key;
            root->value = right_min->value;

            return erase(root->right, right_min->key);
        }
    }
    if (key > root->key){
        return erase(root->right, key);
    }
    return erase(root->left, key);
}

void print(const Node* root, ostream& os=cout){
    if (!root){
        return;
    }
    print(root->left, os);
    os << "(" << root->key << ", " << root->value << ") ";
    print(root->right, os);
}

Node* copy(Node* root){
    if (!root){
        return nullptr;
    }
    Node* result = new Node(root->key, root->value);
    result->left = copy(root->left);
    result->right = copy(root->right);
    return result;
}

int size(Node* root){
    if (!root){
        return 0;
    }
    return 1 + size(root->left) + size(root->right);
}

class map{
    Node* root = nullptr;
    int size = 0;

public:
    map(){
        root = nullptr;
        size = 0;
    }

    map(Node* r, int s){
        if (root){
            delete root;
        }
        root = r;
        size = s;
    }

    map (const map& other){
        if (&other == this){
            return;
        }
        root = ::copy(other.root);
        size = other.size;
        print();
        cout << endl;
    }

    map& operator = (const map& other){
        if (&other == this){
            return *this;
        }
        root = ::copy(other.root);
        size = other.size;
        return *this;
    }

    bool insert(int key, string value){
        bool res = ::insert(root, key, value);
        size += res;
        return res;
    }

    bool erase(int key){
        bool res = ::erase(root, key);
        size -= res;
        return res;
    }

    const string& find(int key) const{
        Node* res = ::find(root, key);
        if (res){
            return res->value;
        }
        static const string empty = "";
        return empty;
    }

    void print(ostream& os = cout) const{
        ::print(root, os);
    }

    ~map(){
        if (size == 0 || (!root) ){
            return;
        }
    
        vector<Node*> nodes = {root};
        int j = 0;
        while (j < nodes.size()){
            Node* cur = nodes[j++];
            if (cur->left){
                nodes.push_back(cur->left);
            }
            if (cur->right){
                nodes.push_back(cur->right);
            }
        }
  
        for (int i = nodes.size() - 1; i >= 0; --i){
            delete nodes[i];
        }
    }

};

void copy_test(void){
    int keys[] = {5, 1, 2, 4, 3};
    string vals[] = {"a", "b", "c", "d", "e"};
    map cpy;
    ostringstream new_os, copy_os;
    {
        map nw;
        for (int i = 0; i < 5; ++i)
        {
            nw.insert(keys[i], vals[i]);
        }
        nw.print(new_os);
        cpy = nw;
    }
    cpy.print(copy_os);
    if (copy_os.str() == new_os.str()){
        cout << "PASSED copy test\n";
    }
    else{
        cout << "ERROR!\n" << copy_os.str() << " != " << new_os.str() << endl;
    }
}


void erase_test(void){
    int keys[] = {5, 1, 2, 4, 3};
    string vals[] = {"a", "b", "c", "d", "e"};

    map m;
    ostringstream os;
    for (int i = 0; i < 5; ++i)
    {
        m.insert(keys[i], vals[i]);
    }
    bool res = m.erase(1);
    m.print(os);
    string ans = "(2, c) (3, e) (4, d) (5, a) ";
    if (ans == os.str() && res == true){
        cout << "PASSED erase test existing\n";
    }else{
        cout << "ERROR!\n" << os.str() << " != " << ans << endl;
    }

    os.str("");

    res = m.erase(6);

    m.print(os);

    if (ans == os.str()&& res == false){
        cout << "PASSED erase test non existing key\n";
    }else{
        cout << "ERROR!\n" << os.str() << " != " << ans << endl;
    }
}

void find_test(void){
    int keys[] = {5, 1, 2, 4, 3};
    string vals[] = {"a", "b", "c", "d", "e"};

    map m;
    for (int i = 0; i < 5; ++i)
    {
        m.insert(keys[i], vals[i]);
    }
    string res = m.find(1);
    if (res == "b"){
        cout << "PASSED find test existing\n";
    }else{
        cout << "ERROR!\n" << res << " != " << "b" << endl;
    }

    res = m.find(6);
    if (res.empty()){
        cout << "PASSED find test non existing key\n";
    }else{
        cout << "ERROR!\n" << res << "isn't empty" << endl;
    }
}

void all_test(){
    insert_test();
    erase_test();
    find_test();
    copy_test();
}

int main(void){
    all_test();
    map m;
    while (true){

        cout << "\n\ninput 1 for insert, 2 for erase, 3 for find, 4 for print, 0 for exit\n";
        int op; cin >> op;
        int key; string value;
        bool res;
        switch(op)
        {
        case 1:
            {
                cout << "input key, value:" << endl;
                cin >> key >> value;
                res = m.insert(key, value);
                if (res){
                    cout << "success!" << endl;
                }else{
                    cout << "this key already in the map!" << endl;
                }
                break;
            }
        case 2:
            {
                cout << "input key:" << endl;
                cin >> key;
                value = m.find(key);
                res = m.erase(key);
                if (res){
                    cout << "erase (" << key << ", " << value << ") from the map!" << endl;
                }else{
                    cout << "no such key in the map!" << endl;
                }
                break;
            }
        case 3:
            {
                cout << "input key:" << endl;
                cin >> key;
                value = m.find(key);
                if (value.empty()){
                    cout << "no such key in the map!" << endl;
                }else{
                    cout << "find (" << key << ", " << value << ") in the map!" << endl;
                }
                break;
            }
        case 4:
            {
                m.print();
                cout << endl;
                break;
            }
        case 0:
            {
                return 0;
            }
        }
    }
}
