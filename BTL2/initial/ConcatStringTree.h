#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"


enum BalanceValue
{
    LH = -1,
    EH = 0,
    RH = 1
};
static int maxId = 0;
/////////////////////////////////////////////BEGIN NODECONCAT////////////////////////////////////////////////
class Node {
public:
    Node() : leftLength(0), length(0), data("\0"), left(NULL), right(NULL), id(0) {
        this->parTree = new ParentsTree();
    }//
   // Node(string data) : leftLength(0), length(data.size()), data(data), left(NULL), right(NULL), parTree(NULL) {} //
    Node(string data) {
        this->leftLength = 0;
        this->length = data.size();
        this->data = data;
        this->left = NULL;
        this->right = NULL;
        this->parTree = new ParentsTree();
        this->id = 0;
    }
    /*Node(char* data) {
        this->leftLength = 0;
        this->length = strlen(data);
        for (int i = 0; i < strlen(data); i++) {
            this->data[i] = data[i];
        }
        this->data[strlen(data)] = '\0';
        //this->data = data;
        this->left = NULL;
        this->right = NULL;
        this->parTree = new ParentsTree();
        this->id = 0;
    }*/
    void delTree(Node*& root) {
        if (root != NULL) {
            Node* delNode = root;
            Node* rootleft = delNode->left;
            delTree(rootleft);
            delNode->left = NULL;
            Node* rootright = delNode->right;
            delTree(rootright);
            delNode->right = NULL;
            delete delNode;
        }
        return;
    }
    ~Node() {
        //this->parTree->delParTree();
        delete this->parTree;
        this->parTree = NULL;
        left = NULL;
        right = NULL;
    }
    //////////////////////////////////////////BEGIN PARENTSTREE//////////////////////////////////////////////
    class ParentsTree {
    public:
        ///////////////////////////////////////////BEGIN PARNODE///////////////////////////////////////////
        class parNode {
        private:
            Node* data;
            int id;
            parNode* left;
            parNode* right;
            BalanceValue balance;
            friend class ParentsTree;
        public:
            parNode() : data(NULL), id(0), left(NULL), right(NULL), balance(EH) {}
            parNode(Node* data) : data(data), id(data->id), left(NULL), right(NULL), balance(EH) {}
            ~parNode() {
                id = 0;
                data = NULL;
                left = NULL;
                right = NULL;
            }
        };
        ////////////////////////////////////////////////END PARNODE////////////////////////////////////////////////
        ParentsTree() : parRoot(NULL), parSize(0) {}
        ParentsTree(Node* dataNode) {
            parNode* newParRoot = new parNode(dataNode);
            this->parRoot = newParRoot;
            this->parSize = 1;
            //root->parTree->insert(root);
            // size = root->data.length();
             //stringtree = root->data;
        }
        int size() const { //O(1)
            return this->parSize;
        }
        string toStringPreOrder() const {
            string s = "ParentsTree[";
            s += toStringPreOrderRec(this->parRoot);
            if (s[s.length() - 1] == ';') s[s.length() - 1] = ']';
            else s += "]";
            return s;
        }
        string toStringPreOrderRec(parNode* root) const {
            string s = "";
            if (root != NULL) {
                s += "(id=";
                s += std::to_string(root->id);
                s += ");";
                s += toStringPreOrderRec(root->left);
                s += toStringPreOrderRec(root->right);
            }
            return s;
        }
        /* int maxId() {
             if (parRoot != NULL) {
                 int max = this->parRoot->id;
                 parNode* temp = this->parRoot;
                 while (temp != NULL) {
                     max = temp->id;
                     temp = temp->right;
                 }
                 return max;
             }
             return 0;
         }*/
        parNode* rotR(parNode*& y) {

            //cout<<"rotateR"<<endl;
            parNode* x = y->left;
            parNode* T2 = x->right;

            // Perform rotation
            x->right = y;
            y->left = T2;

            // Return new root
            return x;
        }
        parNode* rotL(parNode*& x) {
            //cout<<"rotateL"<<endl;
            parNode* y = x->right;
            parNode* T2 = y->left;

            // Perform rotation
            y->left = x;
            x->right = T2;

            // Return new root
            return y;
        }
        /* bool balanceLeft(parNode*& root) {
             // return true if tree get higher
             if (root->balance == EH) {
                 root->balance = LH;
                 return true;
             }
             else if (root->balance == RH) {
                 root->balance = EH;
                 return false;
             }
             else {
                 // imbalanced , root->balance=LH
                 if (root->left->balance == LH) {
                     // L of L
                     root = rotR(root);
                     root->balance = EH;
                     root->right->balance = EH;
                     return false;
                 }
                 else if (root->left->balance == RH) {
                     // R of L
                     root->left = rotL(root->left);
                     root = rotR(root);
                     if (root->balance == EH) {
                         root->left->balance = EH;
                         root->right->balance = EH;
                     }
                     else if (root->balance == LH) {
                         root->balance = EH;
                         root->left->balance = EH; //E->R
                         root->right->balance = RH; //R->E
                     }
                     else {
                         root->balance = EH;
                         root->left->balance = LH; //L->E
                         root->right->balance = EH; //E->L
                     }
                     return false;
                 }
             }
             return 0;
         }*/
        bool balanceRight(parNode*& root) {
            // return true if tree get higher
            if (root->balance == EH) {
                root->balance = RH;
                return true;
            }
            else if (root->balance == LH) {
                root->balance = EH;
                return false;
            }
            else {
                // imbalanced
                if (root->right->balance == RH) {
                    // R of R
                    root = rotL(root);
                    root->balance = EH;
                    root->left->balance = EH;
                    return false;
                }
                else if (root->right->balance == LH) {
                    // L of R
                    root->right = rotR(root->right);
                    root = rotL(root);
                    if (root->balance == EH) {
                        root->right->balance = EH;
                        root->left->balance = EH;
                    }
                    else if (root->balance == RH) {
                        root->balance = EH;
                        root->right->balance = EH; //e->l
                        root->left->balance = LH; //l->e
                    }
                    else {
                        root->balance = EH;
                        root->right->balance = RH; //r->e
                        root->left->balance = EH; //e->r
                    }
                    return false;
                }
            }
            return 0;
        }

        /*bool insert(parNode*& root, Node* data, int id) {
            // return true if tree get higher
            if (root == NULL) {
                parNode* newroot = new parNode(data);
                parSize=1;
                return 1;
            }
            else if (id < root->id) {
                if (insert(root->left, data, id)) {
                    return balanceLeft(root);
                }
                else {
                    return false;
                }
            }
            else {
                // >=
                if (insert(root->right, data, id)) {
                    return balanceRight(root);
                }
                else {
                    return false;
                }
            }
            return 0;
        }*/
        void insert(Node* data) {
            //TODO
            if (this->parSize == 0) {
                parNode* newroot = new parNode(data);
                this->parRoot = newroot;
                this->parSize = 1;
            }
            else {
                //if (this->maxId() + 1 == 10 * 10 * 10 * 10 * 10 * 10 * 10) throw std::overflow_error("Id is overflow!");
                parNode* temp = parRoot;
                while (temp->right != NULL) {
                    temp = temp->right;
                }
                parNode* newnode = new parNode(data);
                //newnode->id = this->maxId() + 1;
                newnode->id = data->id;
                temp->right = newnode;
                balanceRight(this->parRoot);
                this->parSize++;
            }
            return;
        }
        /*int idOfNodeRec(parNode*& root, Node* node) {
            if (root != NULL) {
                if (root->data == node) {
                    return root->id;
                }
                else if (idOfNodeRec(root->left, node) != -1) return idOfNodeRec(root->left, node);
                else return idOfNodeRec(root->right, node);
            }
            return -1;
        }
        int idOfNode(Node* node) {
            return this->idOfNodeRec(this->parRoot, node);
        }*/
        parNode* removeRec(parNode*& root, int id, bool& shorter, bool& success) {
            if (root == NULL) {
                shorter = false;
                success = false;
                return NULL;
            }
            if (id < root->id) {
                root->left = removeRec(root->left, id, shorter, success);
                if (shorter) {
                    root = delrightBalance(root, shorter);
                }
            }
            else if (id > root->id) {
                root->right = removeRec(root->right, id, shorter, success);
                if (shorter) {
                    root = delleftBalance(root, shorter);
                }
            }
            else {
                parNode* delNode = root;
                if (root->right == NULL) {
                    parNode* newRoot = root->left;
                    success = true;
                    shorter = true;
                    delete delNode;
                    return newRoot;
                }
                if (root->left == NULL) {
                    parNode* newRoot = root->right;
                    success = true;
                    shorter = true;
                    delete delNode;
                    return newRoot;
                }
                else {
                    parNode* exchPtr = root->left;
                    while (exchPtr->right != NULL) {
                        exchPtr = exchPtr->right;
                    }
                    root->data = exchPtr->data;
                    root->left = removeRec(root->left, exchPtr->id, shorter, success);
                    if (shorter) {
                        root = delrightBalance(root, shorter);
                    }
                }
            }
            return root;
        }
        parNode* delrightBalance(parNode*& root, bool& shorter) {
            if (root->balance == LH) {
                root->balance = EH;
            }
            else if (root->balance == EH) {
                root->balance = RH;
                shorter = false;
            }
            else {
                parNode* rightTree = root->right;
                if (rightTree->balance == LH) {
                    parNode* leftTree = rightTree->left;
                    if (leftTree->balance == LH) {
                        rightTree->balance = RH;
                        root->balance = EH;
                    }
                    else if (leftTree->balance == EH) {
                        root->balance = LH;
                        rightTree->balance = EH;
                    }
                    else {
                        root->balance = LH;
                        rightTree->balance = EH;
                    }
                    leftTree->balance = EH;
                    root->right = rotR(rightTree);
                    root = rotL(root);
                }
                else {
                    if (rightTree->balance != EH) {
                        root->balance = EH;
                        rightTree->balance = EH;
                    }
                    else {
                        root->balance = RH;
                        rightTree->balance = LH;
                        shorter = false;
                    }
                    root = rotL(root);
                }
            }
            return root;
        }
        parNode* delleftBalance(parNode*& root, bool& shorter) {
            if (root->balance == RH) {
                root->balance = EH;
            }
            else if (root->balance == EH) {
                root->balance = LH;
                shorter = false;
            }
            else {
                parNode* leftTree = root->left;
                if (leftTree->balance == RH) {
                    parNode* rightTree = leftTree->right;
                    if (rightTree->balance == RH) {
                        leftTree->balance = LH;
                        root->balance = EH;
                    }
                    else if (rightTree->balance == EH) {
                        root->balance = RH;
                        leftTree->balance = EH;
                    }
                    else {
                        root->balance = RH;
                        leftTree->balance = EH;
                    }
                    rightTree->balance = EH;
                    root->left = rotL(leftTree);
                    root = rotR(root);
                }
                else {
                    if (leftTree->balance != EH) {
                        root->balance = EH;
                        leftTree->balance = EH;
                    }
                    else {
                        root->balance = LH;
                        leftTree->balance = RH;
                        shorter = false;
                    }
                    root = rotR(root);
                }
            }
            return root;
        }
        void remove(int id) {
            //TODO
            bool shorter = 0, success = 0;
            this->parRoot = removeRec(this->parRoot, id, shorter, success);
            if (success == 1) this->parSize--;

        }
        void delParTreeRec(parNode* root) {
            if (root != NULL) {
                parNode* delParNode = root;
                parNode* rootleft = delParNode->left;
                delParTreeRec(rootleft);
                delParNode->left = NULL;
                parNode* rootright = delParNode->right;
                delParTreeRec(rootright);
                delParNode->right = NULL;
                delete delParNode;
            }
            return;
        }
        ~ParentsTree() {
            delParTreeRec(this->parRoot);
            this->parRoot = NULL;
            this->parSize = 0;
        }

    private:
        parNode* parRoot;
        int parSize;
        friend class ConcatStringTree;
        friend class Node;
    };
    ////////////////////////////////END PARENTSTREE////////////////////////////////////////////////
private:
    int leftLength;
    int length;
    string data;
    Node* left;
    Node* right;
    ParentsTree* parTree;
    int id;
    friend class ConcatStringTree;
    friend class LitStringHash;
    friend class parNode;
    friend class ReducedConcatStringTree;
};
//////////////////////////////////END NODE CONCAT////////////////////////////////////////////////////////


////////////////////////////////////BEGIN CONCATSTRINGTREE///////////////////////////////////////////////
class ConcatStringTree {
    //public:

private:
    Node* root;
    //int size;
public:
    ConcatStringTree() : root(NULL) {
    } //, size(0) , stringtree("\0")
    ConcatStringTree(const char* s) { //O(n)
        Node* newroot = new Node(s);
        newroot->id = ++maxId;
        if (maxId == 10 * 10 * 10 * 10 * 10 * 10 * 10) {
            delete newroot;
            throw std::overflow_error("Id is overflow!");
        }
        root = newroot;
        //ParentsTree* newParTree= new ParentsTree(root);
       // root->parTree = newParTree;
        root->parTree->insert(root);
        // size = root->data.length();
         //stringtree = root->data;
    }
    int length() const { //O(1)
        return this->root->length;
    }
    char get(int index) { //O(logk)
        return getRec(this->root, index);
    }
    char getRec(Node* root, int index) {
        if (index<0 || index>this->length() - 1) throw std::out_of_range("Index of string is invalid!");
        if (root->left == NULL && root->right == NULL) return root->data[index];
        else if (index < root->leftLength) {
            return getRec(root->left, index);
        }
        else return getRec(root->right, index - root->leftLength);
    }
    int indexOf(char c) { //O(l)
        string s = stringtree(this->root);
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == c) return i;
        }
        return -1;
    }
    string stringtree(Node* root) { //lay chuoi ConcatStringTree
        string s = "";
        if (root != NULL) {
            s += stringtree(root->left);
            s += root->data;
            s += stringtree(root->right);
        }
        return s;

    }
    string toStringPreOrder() const { //O(l)
        string s = "ConcatStringTree[";
        s += toStringPreOrderRec(this->root);
        if (s[s.length() - 1] == ';') s[s.length() - 1] = ']';
        else s += "]";
        return s;
    }
    string toStringPreOrderRec(Node* root) const {
        string s = "";
        if (root != NULL) {
            s += "(";
            s += "LL=";
            s += std::to_string(root->leftLength);
            s += ",L=";
            s += std::to_string(root->length);
            s += ",";
            if (root->data == "\0") s += "<NULL>);";
            else {
                s += "\"";
                s += root->data;
                s += "\");";
            }
            s += toStringPreOrderRec(root->left);
            s += toStringPreOrderRec(root->right);
        }
        return s;

    }
    string toString() const { //O(l)
        string s = "ConcatStringTree[\"";
        s += toStringRec(this->root);
        s += "\"]";
        return s;
    }
    string toStringRec(Node* root)const {
        string s = "";
        if (root != NULL) {
            if (root->left != NULL) s += toStringRec(root->left);
            s += root->data;
            if (root->right != NULL) s += toStringRec(root->right);
        }
        return s;
    }
    ConcatStringTree concat(const ConcatStringTree& otherS) const { //O(1)
        Node* newroot = new Node();
        newroot->id = ++maxId;
        if (maxId == 10 * 10 * 10 * 10 * 10 * 10 * 10) {
            delete newroot;
            throw std::overflow_error("Id is overflow!");
        }
        newroot->leftLength = this->length();
        newroot->length = this->length() + otherS.length();
        newroot->left = this->root;
        newroot->right = otherS.root;
        newroot->parTree->insert(newroot);
        newroot->left->parTree->insert(newroot);
        newroot->right->parTree->insert(newroot);
        ConcatStringTree newtree;//= new ConcatStringTree();
        newtree.root = newroot;
        //newtree->stringtree = this->stringtree + otherS.stringtree;
        return newtree;
    }
    Node* rootCopyTree(Node* root) const { //tao cay moi y chang cay cu 
        if (root != NULL) {
            Node* newroot = new Node();
            newroot->data = root->data;
            newroot->leftLength = root->leftLength;
            newroot->length = root->length;
            //newroot->parTree->insert(newroot);
            newroot->id = root->id;
            newroot->left = rootCopyTree(root->left);
            newroot->right = rootCopyTree(root->right);
            if (newroot->left != NULL) newroot->left->parTree->insert(newroot);
            if (newroot->right != NULL) newroot->right->parTree->insert(newroot);
            return newroot;
        }
        return NULL;
    }
    ConcatStringTree subString(int from, int to) const {
        if (from < 0 || from >= length() || to < 0 || to >= length()) throw std::out_of_range("Index of string is invalid!");
        else if (from >= to) throw std::logic_error("Invalid range!");
        ConcatStringTree newtree;//= new ConcatStringTree();
        newtree.root = rootCopyTree(this->root);
        newtree.subStringRec(newtree.root, from, to);
        return newtree;
    }
    void subStringRec(Node*& root, int from, int to) const {
        if (root->left == NULL && root->right == NULL) {
            root->data = root->data.substr(from, to - from);
            root->length = to - from;
            root->leftLength = 0;
            return;
        }
        if (from < root->leftLength) {
            if (to <= root->leftLength) {
                root->right->delTree(root->right);
                root->right = NULL;
                subStringRec(root->left, from, to);
                root->leftLength = root->left->data.length();
            }
            else {
                subStringRec(root->left, from, root->leftLength);
                subStringRec(root->right, 0, to - root->leftLength);
                root->leftLength -= from;
                root->length = to - from;
            }
        }
        else {
            root->delTree(root->left);
            root->left = NULL;
            //int oldleftLength = root->leftLength;
            subStringRec(root->right, 0, to - root->leftLength);
            root->leftLength = 0;
            root->length = to - from;
        }
        return;
    }
    void reverseData(Node*& root) { //Dao cac data cua cac node la
        if (root != NULL) {
            if (root->data != "\0") {
                int l = root->data.length();
                string s = "";
                for (int j = l - 1; j >= 0; j--) {
                    s += root->data[j];
                }
                root->data = s;
            }
            reverseData(root->left);
            reverseData(root->right);
        }
        return;
    }
    Node* rootReverseTree(Node* root) const { //Dao tu phai sang trai
        if (root != NULL) {
            Node* newroot = new Node();
            newroot->data = root->data;
            if (root->right != NULL) newroot->leftLength = root->right->length;
            else newroot->leftLength = 0;
            newroot->length = root->length;
            //newroot->parTree->insert(newroot);
            newroot->id = root->id;
            newroot->left = rootReverseTree(root->right);
            newroot->right = rootReverseTree(root->left);
            if (newroot->left != NULL) newroot->left->parTree->insert(newroot);
            if (newroot->right != NULL) newroot->right->parTree->insert(newroot);
            return newroot;
        }

        return NULL;
    }
    ConcatStringTree reverse() const {
        ConcatStringTree newtree;//= new ConcatStringTree();
        newtree.root = this->rootReverseTree(this->root);
        newtree.reverseData(newtree.root);
        return newtree;
    }
    int getParTreeSize(const string& query) const {
        Node* temp = this->root;
        if (query == "") return temp->parTree->size();
        for (int i = 0; i < query.size(); i++) {
            if (query[i] == 'l') temp = temp->left;
            else if (query[i] == 'r') temp = temp->right;
            else throw std::runtime_error("Invalid character of query");
        }
        if (temp == NULL) {
            throw std::runtime_error("Invalid query: reaching NULL");
        }
        else return temp->parTree->size();
    }
    string getParTreeStringPreOrder(const string& query) const {
        Node* temp = this->root;
        for (int i = 0; i < query.size(); i++) {
            if (query[i] == 'l') temp = temp->left;
            else if (query[i] == 'r') temp = temp->right;
            else throw std::runtime_error("Invalid character of query");
        }
        if (temp == NULL) {
            throw std::runtime_error("Invalid query: reaching NULL");
        }
        else return temp->parTree->toStringPreOrder();
    }
    void delPreOrder(Node*& root) {
        if (root != NULL) {
            Node* delRoot = root;
            Node* delLeft = root->left;
            Node* delRight = root->right;
            if (delRoot->parTree->size() == 0) {
                if (delRoot->left != NULL) {
                    int idRootInLeft = delRoot->id; //delRoot->left->parTree->idOfNode(delRoot);
                    if (idRootInLeft != -1) delRoot->left->parTree->remove(idRootInLeft);
                }
                if (delRoot->right != NULL) {
                    int idRootInRight = delRoot->id; //delRoot->right->parTree->idOfNode(delRoot);
                    if (idRootInRight != -1) delRoot->right->parTree->remove(idRootInRight);
                }
                delete delRoot;
            }
            delPreOrder(delLeft);
            delPreOrder(delRight);
        }
        return;
    }
    ~ConcatStringTree() {
        int index = this->root->id;
        this->root->parTree->remove(index);
        delPreOrder(this->root);
        this->root = NULL;
    }

};
///////////////////////////////////////////////END CONCATSTRINGTREE//////////////////////////////////////

class ReducedConcatStringTree; // forward declaration
class LitStringHash; // forward declaration
///////////////////BEGIN HASHCONFIG//////////////////////////
class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;

    friend class ReducedConcatStringTree;
    friend class LitStringHash;
public:
    HashConfig() : p(0), c1(0), c2(0), lambda(0), alpha(0), initSize(0) {}
    HashConfig(int p, double c1, double c2, double lambda, int alpha, int initSize) {
        this->p = p;
        this->c1 = c1;
        this->c2 = c2;
        this->lambda = lambda;
        this->alpha = alpha;
        this->initSize = initSize;
    }
    ~HashConfig() {
        p = c1 = c2 = lambda = alpha = initSize = 0;
    }
};
//////////////////END HASHCONFIG/////////////////////////////

/////////////////////BEGIN LITSTRINGHASH////////////////////
enum STATUS_TYPE { NIL, NON_EMPTY, DELETED };
class LitStringHash {
public:
    ////////////////////BEGIN LITSTRING NODE///////////////////
    class litString {
    private:
        //Node* litStringNode;
        string str;
        int numLink;
        friend class LitStringHash;
        friend class ReducedConcatStringTree;
    public:
        litString() : str("\0"), numLink(0) {}
        litString(string str) : str(str), numLink(1) {}
        ~litString() {
            str = "\0";
            numLink = 0;
        }
    };
    ///////////////////END LITSTRING NODE/////////////////////
private:
    litString* litStringArr;
    HashConfig hashConfig;
    int lastInsertedIndex;
    STATUS_TYPE* status;
    int size;
    int numOfElements;
    friend class ReducedConcatStringTree;
public:
    LitStringHash(const HashConfig& hashConfig) {
        this->hashConfig = hashConfig;
        this->size = hashConfig.initSize;
        this->litStringArr = new litString[hashConfig.initSize];
        this->status = new STATUS_TYPE[hashConfig.initSize];
        for (int i = 0; i < hashConfig.initSize; i++) {
            this->status[i] = NIL;
        }
        this->numOfElements = 0;
    }
    int getLastInsertedIndex() const {
        return this->lastInsertedIndex;
    }
    string toString() const {
        string s = "LitStringHash[";
        for (int i = 0; i < this->hashConfig.initSize; i++) {
            s += "(";
            if (this->litStringArr[i].str != "\0") {
                s += "litS=\"";
                s += this->litStringArr[i].str;
                s += "\"";
            }
            s += ");";
        }
        if (s[s.length() - 1] == ';') s[s.length() - 1] = ']';
        else s += "]";
        return s;
    }
    int h(string s) {
        int h = 0;
        int p = 1;
        for (int i = 0; i < s.size(); i++) {
            h += int(s[i]) * p;
            p *= this->hashConfig.p;
        }
        return h % this->hashConfig.initSize;
    }
    int hp(string s, int i) {
        int hp = h(s) + this->hashConfig.c1 * i + this->hashConfig.c2 * i * i;
        return hp % this->hashConfig.initSize;
    }
    int insertLitString(string s) {
        int i = 0;
        do {
            int slot = this->hp(s, i);
            if (status[slot] == NIL || status[slot] == DELETED) {
                litStringArr[slot].str = s;
                litStringArr[slot].numLink = 1;
                status[slot] = NON_EMPTY;
                this->lastInsertedIndex = slot;
                this->numOfElements++;
                rehash();
                return slot;
            }
            else i++;
        } while (i < this->size);
        throw std::runtime_error("No possible slot");
        return 0;
    }
    void removeLitString(string s) {
        int slot = this->searchLitString(s);
        if (slot == -1) throw "Not found!";
        status[slot] = DELETED;
        this->numOfElements--;
        if (numOfElements == 0) {
            delete[] litStringArr;
            delete[] status;
        }
        return;
    }
    int searchLitString(string s) {
        int i = 0;
        do {
            int slot = this->hp(s, i);
            if (status[slot] == NIL) return -1;
            if (litStringArr[slot].str == s && status[slot] == NON_EMPTY) return slot;
            i++;
        } while (i < this->size);
        return -1;
    }
    void rehash() {
        double loadFactor = this->numOfElements / this->size;
        if (loadFactor > this->hashConfig.lambda) {
            int newsize = this->size * this->hashConfig.alpha;
            HashConfig* newHC = new HashConfig(hashConfig.p, hashConfig.c1, hashConfig.c2, hashConfig.lambda, hashConfig.alpha, newsize);
            LitStringHash* newLSH = new LitStringHash(*newHC);
            for (int i = 0; i < hashConfig.initSize; i++) {
                if (this->status[i] == NON_EMPTY) {
                    newLSH->insertLitString(this->litStringArr[i].str);
                }
            }
            this->litStringArr = newLSH->litStringArr;;
            this->hashConfig = *newHC;
            this->lastInsertedIndex = newLSH->lastInsertedIndex;
            this->status = newLSH->status;
            this->size = newsize;
            this->numOfElements = newLSH->numOfElements;
        }
        return;
    }
    ~LitStringHash() {
        //delete[] litStringArr;
        //delete[] status;
        status = NULL;
        litStringArr = NULL;
        size = 0;
        numOfElements = 0;
        lastInsertedIndex = 0;
    }
};
/////////////////////END LITSTRINGHASH////////////////////

/////////////////////BEGIN ReducedConcatStringTree////////////
static int maxIdRed = 0;
class ReducedConcatStringTree {//:public ConcatStringTree
private:
    Node* root;
public:
    ReducedConcatStringTree() : root(NULL) {}
    LitStringHash* litStringHash;
    ReducedConcatStringTree(const char* s, LitStringHash* litStringHash) {
        this->litStringHash = litStringHash;
        if (litStringHash->searchLitString(s) != -1) {
            int index = litStringHash->searchLitString(s);
            Node* newroot = new Node(litStringHash->litStringArr[index].str);
            newroot->id = ++maxIdRed;
            if (maxIdRed == 10 * 10 * 10 * 10 * 10 * 10 * 10) {
                delete newroot;
                throw std::overflow_error("Id is overflow!");
            }
            litStringHash->litStringArr[index].numLink++;
            root = newroot;
            root->parTree->insert(root);
        }
        else {
            Node* newroot = new Node(s);
            newroot->id = ++maxIdRed;
            if (maxIdRed == 10 * 10 * 10 * 10 * 10 * 10 * 10) {
                delete newroot;
                throw std::overflow_error("Id is overflow!");
            }
            //if (maxIdRed == 10 * 10 * 10 * 10 * 10 * 10 * 10) throw std::overflow_error("Id is overflow!");
            root = newroot;
            root->parTree->insert(root);
            litStringHash->insertLitString(s);
        }

    }
    int length() const { //O(1)
        return this->root->length;
    }
    char get(int index) { //O(logk)
        return getRec(this->root, index);
    }
    char getRec(Node* root, int index) {
        if (index<0 || index>this->length() - 1) throw std::out_of_range("Index of string is invalid!");
        if (root->left == NULL && root->right == NULL) return root->data[index];
        else if (index < root->leftLength) {
            return getRec(root->left, index);
        }
        else return getRec(root->right, index - root->leftLength);
    }
    int indexOf(char c) { //O(l)
        string s = stringtree(this->root);
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == c) return i;
        }
        return -1;
    }
    string stringtree(Node* root) { //lay chuoi ConcatStringTree
        string s = "";
        if (root != NULL) {
            s += stringtree(root->left);
            s += root->data;
            s += stringtree(root->right);
        }
        return s;

    }
    string toStringPreOrder() const { //O(l)
        string s = "ConcatStringTree[";
        s += toStringPreOrderRec(this->root);
        if (s[s.length() - 1] == ';') s[s.length() - 1] = ']';
        else s += "]";
        return s;
    }
    string toStringPreOrderRec(Node* root) const {
        string s = "";
        if (root != NULL) {
            s += "(";
            s += "LL=";
            s += std::to_string(root->leftLength);
            s += ",L=";
            s += std::to_string(root->length);
            s += ",";
            if (root->data == "\0") s += "<NULL>);";
            else {
                s += "\"";
                s += root->data;
                s += "\");";
            }
            s += toStringPreOrderRec(root->left);
            s += toStringPreOrderRec(root->right);
        }
        return s;

    }
    string toString() const { //O(l)
        string s = "ConcatStringTree[\"";
        s += toStringRec(this->root);
        s += "\"]";
        return s;
    }
    string toStringRec(Node* root)const {
        string s = "";
        if (root != NULL) {
            if (root->left != NULL) s += toStringRec(root->left);
            s += root->data;
            if (root->right != NULL) s += toStringRec(root->right);
        }
        return s;
    }
    ReducedConcatStringTree concat(const ReducedConcatStringTree& otherS) const { //O(1)
        Node* newroot = new Node();
        newroot->id = ++maxIdRed;
        if (maxIdRed == 10 * 10 * 10 * 10 * 10 * 10 * 10) {
            delete newroot;
            throw std::overflow_error("Id is overflow!");
        }
        newroot->leftLength = this->length();
        newroot->length = this->length() + otherS.length();
        newroot->left = this->root;
        newroot->right = otherS.root;
        newroot->parTree->insert(newroot);
        newroot->left->parTree->insert(newroot);
        newroot->right->parTree->insert(newroot);
        ReducedConcatStringTree newtree; //= new ReducedConcatStringTree();
        newtree.root = newroot;
        //newtree->stringtree = this->stringtree + otherS.stringtree;
        return newtree;
    }
    Node* rootCopyTree(Node* root) const { //tao cay moi y chang cay cu 
        if (root != NULL) {
            Node* newroot = new Node();
            newroot->data = root->data;
            newroot->leftLength = root->leftLength;
            newroot->length = root->length;
            //newroot->parTree->insert(newroot);
            newroot->id = root->id;
            newroot->left = rootCopyTree(root->left);
            newroot->right = rootCopyTree(root->right);
            if (newroot->left != NULL) newroot->left->parTree->insert(newroot);
            if (newroot->right != NULL) newroot->right->parTree->insert(newroot);
            return newroot;
        }
        return NULL;
    }
    ReducedConcatStringTree subString(int from, int to) const { //not test
        if (from < 0 || from >= length() || to < 0 || to >= length()) throw std::out_of_range("Index of string is invalid!");
        else if (from >= to) throw std::logic_error("Invalid range!");
        ReducedConcatStringTree newtree;//= new ReducedConcatStringTree();
        newtree.root = rootCopyTree(this->root);
        newtree.subStringRec(newtree.root, from, to);
        return newtree;
    }
    void subStringRec(Node*& root, int from, int to) const {
        if (root->left == NULL && root->right == NULL) {
            root->data = root->data.substr(from, to - from);
            root->length = to - from;
            return;
        }
        if (from < root->leftLength) {
            if (to <= root->leftLength) {
                root->right->delTree(root->right);
                root->right = NULL;
                subStringRec(root->left, from, to);
                root->leftLength = root->left->data.length();
            }
            else {
                subStringRec(root->left, from, root->leftLength);
                subStringRec(root->right, 0, to - root->leftLength);
                root->leftLength -= from;
                root->length = to - from;
            }
        }
        else {
            root->delTree(root->left);
            root->left = NULL;
            //int oldleftLength = root->leftLength;
            subStringRec(root->right, 0, to - root->leftLength);
            root->leftLength = 0;
            root->length = to - from;
        }
        return;
    }
    void reverseData(Node*& root) { //Dao cac data cua cac node la
        if (root != NULL) {
            if (root->data != "\0") {
                int l = root->data.length();
                string s = "";
                for (int j = l - 1; j >= 0; j--) {
                    s += root->data[j];
                }
                root->data = s;
            }
            reverseData(root->left);
            reverseData(root->right);
        }
        return;
    }
    Node* rootReverseTree(Node* root) const { //Dao tu phai sang trai
        if (root != NULL) {
            Node* newroot = new Node();
            newroot->data = root->data;
            if (root->right != NULL) newroot->leftLength = root->right->length;
            else newroot->leftLength = 0;
            newroot->length = root->length;
            //newroot->parTree->insert(newroot);
            newroot->id = root->id;
            newroot->left = rootReverseTree(root->right);
            newroot->right = rootReverseTree(root->left);
            if (newroot->left != NULL) newroot->left->parTree->insert(newroot);
            if (newroot->right != NULL) newroot->right->parTree->insert(newroot);
            return newroot;
        }

        return NULL;
    }
    ReducedConcatStringTree reverse() const { //not test
        ReducedConcatStringTree newtree; //= new ReducedConcatStringTree();
        newtree.root = this->rootReverseTree(this->root);
        newtree.reverseData(newtree.root);
        return newtree;
    }
    int getParTreeSize(const string& query) const {
        Node* temp = this->root;
        if (query == "") return temp->parTree->size();
        for (int i = 0; i < query.size(); i++) {
            if (query[i] == 'l') temp = temp->left;
            else if (query[i] == 'r') temp = temp->right;
            else throw std::runtime_error("Invalid character of query");
        }
        if (temp == NULL) {
            throw std::runtime_error("Invalid query: reaching NULL");
        }
        else return temp->parTree->size();
    }
    string getParTreeStringPreOrder(const string& query) const {
        Node* temp = this->root;
        for (int i = 0; i < query.size(); i++) {
            if (query[i] == 'l') temp = temp->left;
            else if (query[i] == 'r') temp = temp->right;
            else throw std::runtime_error("Invalid character of query");
        }
        if (temp == NULL) {
            throw std::runtime_error("Invalid query: reaching NULL");
        }
        else return temp->parTree->toStringPreOrder();
    }
    void delPreOrder(Node*& root) {
        if (root != NULL) {
            Node* delRoot = root;
            Node* delLeft = root->left;
            Node* delRight = root->right;
            if (delRoot->parTree->size() == 0) {
                if (delRoot->left != NULL) {
                    int idRootInLeft = delRoot->id; //delRoot->left->parTree->idOfNode(delRoot);
                    if (idRootInLeft != -1) delRoot->left->parTree->remove(idRootInLeft);
                }
                if (delRoot->right != NULL) {
                    int idRootInRight = delRoot->id; //delRoot->right->parTree->idOfNode(delRoot);
                    if (idRootInRight != -1) delRoot->right->parTree->remove(idRootInRight);
                }
                if (delRoot->data != "\0") {
                    int index = litStringHash->searchLitString(delRoot->data);
                    litStringHash->litStringArr[index].numLink--;
                    if (litStringHash->litStringArr[index].numLink == 0) litStringHash->removeLitString(delRoot->data);
                    //if (litStringHash->numOfElements == 0) delete litStringHash;
                }
                delete delRoot;
            }
            delPreOrder(delLeft);
            delPreOrder(delRight);
        }
        return;
    }
    ~ReducedConcatStringTree() {
        int index = this->root->id;
        this->root->parTree->remove(index);
        delPreOrder(this->root);
        if (litStringHash->numOfElements == 0) litStringHash = NULL;
        this->root = NULL;
    }
};
//////////////////////END ReducedConcatStringTree//////////////////


#endif // __CONCAT_STRING_TREE_H__
