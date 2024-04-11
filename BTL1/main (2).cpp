#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class ConcatStringList {
public:
    class ReferencesList; // forward declaration
    class DeleteStringList; // forward declaration

public:
    static ReferencesList refList;
    static DeleteStringList delStrList;

    // TODO: may provide some attributes
private:
    class CharALNode {
    public:
        string CharArrayList;
        CharALNode* next;
        bool remove = 0;
    };
private:
    CharALNode* head;
    CharALNode* tail;
    int count;
    int size;

public:
    ConcatStringList() : head(NULL), tail(NULL), count(0), size(0) {}
    ConcatStringList(const char* s) { //O(n)
        CharALNode* node = new CharALNode;
        node->CharArrayList = s;
        node->next = NULL;
        head = tail = node;
        this->count = 1;
        this->size = node->CharArrayList.length();
        //Ref
        this->refList.addRnode(this->head);
        this->refList.addRnode(this->tail);
    }
    int length() const {
        /*CharALNode* temp = head;
        int l = 0;
        for (int i = 0; i < count; i++) {
            l += temp->CharArrayList.length();
            temp = temp->next;
        }*/
        return this->size;
    }
    void add(string s) { //O(1)
        if (this->count == 0) {
            CharALNode* temp = new CharALNode();
            temp->CharArrayList = s;
            temp->next = NULL;
            head = tail = temp;
            this->count++;
            this->size += temp->CharArrayList.length();
        }
        else {
            CharALNode* temp = new CharALNode();
            temp->CharArrayList = s;
            temp->next = NULL;
            tail->next = temp;
            tail = temp;
            this->count++;
            this->size += temp->CharArrayList.length();
        }
        return;
    }
    char get(int index) const { //O(k), k: so luong CharALNode
        if (index < 0 || index >= this->length()) throw std::out_of_range("Index of string is invalid!");
        string s = "";
        CharALNode* temp = head;
        for (int i = 0; i < count; i++) {
            s += temp->CharArrayList;
            temp = temp->next;
        }
        return s[index];
    }
    int indexOf(char c) const {  //O(l), l: chieu dai chuoi ConcatStringList
        string s = "";
        CharALNode* temp = head;
        for (int i = 0; i < count; i++) {
            s += temp->CharArrayList;
            temp = temp->next;
        }
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == c) return i;
        }
        return -1;
    }
    std::string toString() const { //O(l), l: chieu dai chuoi ConcatStringList
        string s = "";
        CharALNode* temp = head;
        while (temp != NULL) {
            s += temp->CharArrayList;
            if (temp==tail) break;
            temp = temp->next;
        }
        //delete temp;
        return s;
    }
    ConcatStringList concat(const ConcatStringList& otherS) const { //O(1)
        ConcatStringList* snew = new ConcatStringList();
       /* CharALNode* temp = head;
        while (temp != NULL) {
            snew->add(temp->CharArrayList);
            temp = temp->next;
        }*/
        snew->head = this->head;
        this->tail->next = otherS.head;
        snew->tail = otherS.tail;
        snew->count = this->count + otherS.count;
        snew->size = this->size + otherS.size;
        //Ref
        this->refList.addRnode(snew->head);
        this->refList.addRnode(snew->tail);
        //
        return *snew;
    }
    ConcatStringList subString(int from, int to) const {
        if (from < 0 || from >= length() || to < 0 || to >= length()) throw std::out_of_range("Index of string is invalid");
        else if (from >= to) throw std::logic_error("Invalid range");
        ConcatStringList* snew = new ConcatStringList();
        CharALNode* temp = head;
        while (temp != NULL) {
            snew->add(temp->CharArrayList);
            temp = temp->next;
        }
        int end = 0;
        int start = 0;
        temp = snew->head;
        while (temp != NULL) {
            //for (int i = 0; i < count; i++) {
            end += temp->CharArrayList.length();
            if (from < end) {
                snew->head = temp;
                if (to < end) {
                    snew->tail = temp;
                    snew->count = 1;
                    temp->CharArrayList = temp->CharArrayList.substr(from - start, to - from);
                    //snew.size = temp->CharArrayList.length();
                    snew->size = to - from;
                    //Ref
                    this->refList.addRnode(snew->head);
                    this->refList.addRnode(snew->tail);
                    //
                    return *snew;
                }
                else {
                    int l = temp->CharArrayList.length();
                    temp->CharArrayList = temp->CharArrayList.substr(from - start, end);
                    //snew.size += temp->CharArrayList.length();
                    start += l;
                    temp = temp->next;
                    snew->count = 2;
                    end += temp->CharArrayList.length();
                    while (temp != NULL) {
                        if (to < end) {
                            temp->CharArrayList = temp->CharArrayList.substr(0, to - start);
                            //snew.size += temp->CharArrayList.length();
                            snew->tail = temp;
                            snew->size = to - from;
                            //Ref
                            this->refList.addRnode(snew->head);
                            this->refList.addRnode(snew->tail);
                            //
                            return *snew;
                        }
                        start += temp->CharArrayList.length();
                        snew->count++;
                        snew->size += temp->CharArrayList.length();
                        temp = temp->next;
                        end += temp->CharArrayList.length();
                    }
                }
            }
            else {
                start += temp->CharArrayList.length();
                temp = temp->next;
            }
        }
        return *snew;
    }
    ConcatStringList reverse() const {
        ConcatStringList* snew = new ConcatStringList();
        CharALNode* temp = head;
        while (temp != NULL) {
            snew->add(temp->CharArrayList);
            if (temp==tail) break;
            temp = temp->next;
        }
        temp = snew->head;
        CharALNode* prev = NULL;
        CharALNode* next = NULL;
        while (temp != NULL) {
            next = temp->next;
            temp->next = prev;
            prev = temp;
            temp = next;
        }

        snew->head = prev;
        snew->count = this->count;
        temp = snew->head;
        int i = 0;
        while (temp != NULL) {
            if (i == snew->count - 1) {
                snew->tail = temp;
                break;
            }
            i++;
        }
        temp = snew->head;
        while (temp != NULL) {
            int l = temp->CharArrayList.length();
            string s = "";
            for (int j = l - 1; j >= 0; j--) {
                s += temp->CharArrayList[j];
            }
            temp->CharArrayList = s;
            temp = temp->next;
        }
        //Ref
        this->refList.addRnode(snew->head);
        this->refList.addRnode(snew->tail);
        //
        return *snew;
    }
    ~ConcatStringList() {
        //cout << "da xoa gi do"<<endl;
       //cout << this->head->CharArrayList << endl;
       //cout << this->tail->CharArrayList << endl;
        RNode* temp = this->refList.Rhead;
        while (temp != NULL) {
            if (temp->node == this->head) {
                temp->numref--;
                break;
            }
            temp = temp->Rnext;
        }
        RNode* temp1 = this->refList.Rhead;
        while (temp1 != NULL) {
            if (temp1->node == this->tail) {
                temp1->numref--;
                break;
            }
            temp1 = temp1->Rnext;
        }
        this->refList.Rsort();
        this->delStrList.addDnode(temp, temp1);
        //cout<<"Dcount: "<<this->delStrList.Dcount<<endl;
        if (this->delStrList.Dcount > 0) {
            DNode* temp2 = this->delStrList.Dhead;
            while (temp2 != NULL) {
                // cout<<"vo while"<<endl;
                if (temp2->nodehead != nullptr && temp2->nodetail != nullptr) {
                    //cout<<"vo if 1"<<endl;
                    if ((temp2->nodehead->numref + temp2->nodetail->numref) == 0 && temp2->nodehead->node->remove == 0 && temp2->nodetail->node->remove == 0) {
                        //cout<<"vo if 2"<<endl;
                        //cout << temp2->nodehead->node->CharArrayList<< endl;
                        CharALNode* temp3 = temp2->nodehead->node;
                        CharALNode* temp4;
                        while (temp3 != temp2->nodetail->node) {
                            //cout << temp3->CharArrayList << endl;
                            temp4 = temp3;
                            temp4->remove = 1;
                            temp3 = temp3->next;
                            //temp3->remove = 1;

                            //cout <<"xoa chuoi: "<< temp4->CharArrayList << endl;
                            temp4 = nullptr;
                            //if (temp4!=nullptr) 
                            delete temp4;
                        }

                        //cout <<"xoa chuoi: "<< temp3->CharArrayList << endl;
                        temp3 = nullptr;
                        //if (temp3!=nullptr) 
                        delete temp3;
                        /*DNode* temp5 = temp2;
                        temp2 = temp2->Dnext;
                        //cout << "abc" << endl;
                        this->delStrList.removeDnode(temp5);
                        cout << "xoa Dnode" << endl;
                        if (this->delStrList.Dcount == 0) {
                            cout << "xoa Ref" << endl;
                            this->refList.removeallRnode();
                        }
                        //cout << "temp2: " << temp2->nodehead->node->CharArrayList << endl;
                        continue;*/
                    }

                }
                temp2 = temp2->Dnext;

            }
            temp2 = this->delStrList.Dhead;
            while (temp2 != NULL) {
                if (temp2->nodehead != nullptr && temp2->nodetail != nullptr) {
                    if ((temp2->nodehead->numref + temp2->nodetail->numref) == 0) {
                        DNode* temp5 = temp2;
                        temp2 = temp2->Dnext;
                        //cout << "abc" << endl;
                        this->delStrList.removeDnode(temp5);
                        //cout << "xoa Dnode" << endl;
                        if (this->delStrList.Dcount == 0) {
                            //cout << "xoa Ref" << endl;
                            this->refList.removeallRnode();
                        }
                        //cout << "temp2: " << temp2->nodehead->node->CharArrayList << endl;
                        continue;
                    }
                }

                temp2 = temp2->Dnext;
            }
        }
        //cout<<"Dcount: "<<this->delStrList.Dcount<<endl;
    }
public:
    class RNode {
    public:
        CharALNode* node;
        int numref;
        RNode* Rnext;
    };
    class DNode {
    public:
        RNode* nodehead;
        RNode* nodetail;
        DNode* Dnext;
    };
    class ReferencesList {
        // TODO: may provide some attributes

    public:
        RNode* Rhead;
        RNode* Rtail;
        int Rcount;

    public:
        ReferencesList() : Rhead(NULL), Rtail(NULL), Rcount(0) {}
        int size() const { //O(1)
            return this->Rcount;
        }
        int refCountAt(int index) const {
            if (index < 0 || index >= this->Rcount) {
                throw std::out_of_range("Index of references list is invalid!");
            }
            RNode* temp = this->Rhead;
            for (int i = 0; i < index; i++) {
                temp = temp->Rnext;
            }
            return temp->numref;
        }
        std::string refCountsString() const { //O(n)
            string str = "RefCounts[";
            if (Rcount > 0) {
                RNode* temp = Rhead;
                for (int i = 0; i < Rcount - 1; i++) {
                    str += (char)temp->numref + 48;
                    //str += temp->node->CharArrayList;
                    str += ",";
                    temp = temp->Rnext;
                }
                str += (char)temp->numref + 48;
                //str += temp->node->CharArrayList;
            }
            str += "]";
            return str;
        }
        void addRnode(CharALNode* node) {
            if (this->Rcount == 0) {
                RNode* newnode = new RNode();
                newnode->node = node;
                newnode->numref = 1;
                newnode->Rnext = NULL;
                this->Rhead = this->Rtail = newnode;
                this->Rcount++;
                return;
            }
            else {
                RNode* temp = this->Rhead;
                while (temp != NULL) {
                    if (node->CharArrayList== temp->node->CharArrayList) {
                        cout<<"co giong "<<temp->node->CharArrayList<<endl;
                        temp->numref++;
                       this->Rsort();
                        return;
                    }
                    temp = temp->Rnext;
                }
                RNode* temp1 = new RNode();
                temp1->node = node;
                temp1->Rnext = Rhead;
                temp1->numref = 1;
                Rhead = temp1;
                cout<<"tao moi "<<temp1->node->CharArrayList<<endl;
                this->Rcount++;
               this->Rsort();
            }
            return;
        }
        void Rsort() {
            if (Rcount > 1 && Rhead != nullptr) {
                RNode* temp = Rhead;
                for (int i = 0; i < Rcount - 1 && temp != nullptr; i++) {
                    if (temp->numref == 0) {
                        if (temp == Rhead) {
                            Rhead = temp->Rnext;
                            Rtail->Rnext = temp;
                            temp->Rnext = NULL;
                            Rtail = temp;
                        }
                        else {
                            RNode* prenode = Rhead;
                            int id = i;
                            for (int j = 0; j < id - 1; j++) {
                                prenode = prenode->Rnext;
                            }
                            prenode->Rnext = temp->Rnext;
                            Rtail->Rnext = temp;
                            temp->Rnext = NULL;
                            Rtail = temp;
                        }

                        /*RNode* temp0 = temp;
                        int id = i;
                        RNode* temp1 = temp0->Rnext;
                        for (int j = id + 1; j < Rcount; j++) {
                            if (temp1->numref != 0) {
                                if (Rcount == 2) {
                                    temp1->Rnext = temp;
                                    temp->Rnext = NULL;
                                    Rhead = temp1;
                                    Rtail = temp;
                                }
                                else {
                                    RNode* prenode = Rhead;
                                    for (int k = 0; k < id - 1; i++) {
                                        prenode = prenode->Rnext;
                                    }
                                    if (j <= Rcount - 2) {
                                        RNode* postnode = prenode->Rnext->Rnext->Rnext;
                                        prenode->Rnext = temp1;
                                        temp1->Rnext = temp0;
                                        temp0->Rnext = postnode;
                                    }
                                    else if (j == Rcount - 1) {
                                        prenode->Rnext = temp1;
                                        temp1->Rnext = temp0;
                                        temp0->Rnext = NULL;
                                        Rtail = temp0;
                                    }
                                }
                                //CharALNode* tempnode = temp->node;
                                //int tempnumref = temp->numref;
                               // temp0->node = temp1->node;
                               // temp0->numref = temp1->numref;
                                //temp1->node = tempnode;
                                //temp1->numref = tempnumref;
                            }
                            temp0 = temp1;
                            temp1 = temp1->Rnext;
                            id++;
                        }*/
                    }
                    temp = temp->Rnext;

                }
                temp = Rhead;
                for (int i = 0; i < Rcount - 1 && temp != nullptr; i++) {
                    RNode* temp1 = temp->Rnext;
                    for (int j = i + 1; j < Rcount && temp1 != nullptr; j++) {
                        if (temp1->numref < temp->numref && temp1->numref != 0 && temp->numref != 0) {
                            if (Rcount == 2) {
                                temp1->Rnext = temp;
                                temp->Rnext = NULL;
                                Rhead = temp1;
                                Rtail = temp;
                            }
                            else if (temp == Rhead) {
                                RNode* postnode = temp1->Rnext;
                                temp->Rnext = postnode;
                                temp1->Rnext = temp;
                                Rhead = temp1;
                            }
                            else {
                                RNode* prenode = Rhead;
                                for (int k = 0; k < i - 1; k++) {
                                    prenode = prenode->Rnext;
                                }
                                if (j <= Rcount - 2) {
                                    RNode* postnode = prenode->Rnext->Rnext->Rnext;
                                    prenode->Rnext = temp1;
                                    temp1->Rnext = temp;
                                    temp->Rnext = postnode;
                                }
                                else {
                                    prenode->Rnext = temp1;
                                    temp1->Rnext = temp;
                                    temp->Rnext = NULL;
                                    Rtail = temp;
                                }
                            }
                            /*CharALNode* tempnode = temp->node;
                            int tempnumref = temp->numref;
                            temp->node = temp1->node;
                            temp->numref = temp1->numref;
                            temp1->node = tempnode;
                            temp1->numref = tempnumref;*/
                        }
                        temp1 = temp1->Rnext;
                    }
                    temp = temp->Rnext;
                }
            }
            return;
        }
        void removeallRnode() {
            if (Rcount > 0 && Rhead != nullptr) {
                RNode* temp3 = Rhead;
                RNode* temp4;
                while (temp3 != NULL) {
                    temp4 = temp3;
                    temp3 = temp3->Rnext;
                    delete temp4;
                }
                Rcount = 0;
                Rhead = Rtail = NULL;
            }
            return;
        }
    };

    class DeleteStringList {
        // TODO: may provide some attributes

    public:
        DNode* Dhead;
        DNode* Dtail;
        int Dcount;
    public:
        DeleteStringList() : Dhead(NULL), Dtail(NULL), Dcount(0) {}
        int size() const { //O(1)
            return this->Dcount;
        }
        std::string totalRefCountsString() const { //O(n)
            string str = "TotalRefCounts[";
            /*int sum = 0;
            if (Dcount > 0) {
                RNode* Rtemp = refList.Rhead;
                while (Rtemp!=NULL){
                   // str += temp->nodetail->node->CharArrayList;
                    //str += ",";
                    DNode* Dtemp = Dhead;
                    while (Dtemp != NULL) {
                        if (Dtemp->nodehead == Rtemp || Dtemp->nodetail == Rtemp) {
                            sum += Rtemp->numref;
                            break;
                        }
                        Dtemp = Dtemp->Dnext;
                    }
                    Rtemp = Rtemp->Rnext;
                }
                //str += temp->nodehead->node->CharArrayList;
            }
            if (sum>0) str += (char)sum + 48;*/
            if (Dcount > 0) {
                DNode* temp = Dhead;
                for (int i = 0; i < Dcount - 1; i++) {
                    if (temp->nodehead != temp->nodetail)
                        str += (char)(temp->nodehead->numref + temp->nodetail->numref) + 48;
                    else str += (char)(temp->nodehead->numref) + 48;
                    str += ",";
                    temp = temp->Dnext;
                }
                if (temp->nodehead != temp->nodetail)
                    str += (char)(temp->nodehead->numref + temp->nodetail->numref) + 48;
                else str += (char)(temp->nodehead->numref) + 48;
            }
            //str += ",";
            //str += (char)Dcount + 48;
            str += "]";
            return str;
        }
        void addDnode(RNode* nodehead, RNode* nodetail) {
            if (this->Dcount == 0) {
                DNode* newnode = new DNode();
                newnode->nodehead = nodehead;
                newnode->nodetail = nodetail;
                newnode->Dnext = NULL;
                this->Dhead = this->Dtail = newnode;
                this->Dcount = 1;
                //cout << Dhead->nodehead->node->CharArrayList << endl;
                //cout << Dtail->nodetail->node->CharArrayList << endl;
                return;
            }
            else {
                DNode* temp1 = new DNode();
                temp1->nodehead = nodehead;
                temp1->nodetail = nodetail;
                temp1->Dnext = NULL;
                Dtail->Dnext = temp1;
                Dtail = temp1;
                this->Dcount++;
            }
            return;
        }
        void removeDnode(DNode* dnode) {

            if (Dcount == 1) {
                if (Dhead == dnode) {
                    delete dnode;
                    Dcount = 0;
                    Dhead = Dtail = NULL;
                    return;
                }
            }
            else if (Dhead == dnode) {
                Dhead = dnode->Dnext;
                delete dnode;
                Dcount--;
                return;
            }
            else if (Dtail == dnode) {
                DNode* temp = Dhead;
                while (temp->Dnext != Dtail) {
                    temp = temp->Dnext;
                }
                temp->Dnext = NULL;
                delete Dtail;
                Dtail = temp;
                Dcount--;
                return;
            }
            DNode* temp = Dhead;
            while (temp->Dnext != dnode) {
                temp = temp->Dnext;
            }
            temp->Dnext = dnode->Dnext;
            delete dnode;
            Dcount--;
            return;
        }
    };
};
ConcatStringList::ReferencesList ConcatStringList::refList = ConcatStringList::ReferencesList();
ConcatStringList::DeleteStringList ConcatStringList::delStrList = ConcatStringList::DeleteStringList();
int main() {
    //ConcatStringList s1("Hello,_world");
    //// test length
    //cout << "s1's length: " << s1.length() << endl;
    //// test get
    //cout << "char at index 2: " << s1.get(2) << endl;
    //try {
    //    cout << "char at index 22: " << s1.get(22) << endl;
    //}
    //catch (out_of_range& ofr) {
    //    cout << "Exception out_of_range: " << ofr.what() << endl;
    //}
    //// test indexOf
    //cout << "in s1, index of 'o': " << s1.indexOf('o') << endl;
    //cout << "in s1, index of 'a': " << s1.indexOf('a') << endl;
    //// test toString
    //cout << "s1 toString: " << s1.toString() << endl;

   // ConcatStringList s1("Hello,");
   // ConcatStringList s2("_this_is");
   // ConcatStringList s3("_an_assignment");

   // // test concat
   // ConcatStringList s4 = s1.concat(s2);
   // cout << "s4: " << s4.toString() << endl;

   // //ConcatStringList s5 = s1.concat(s2).concat(s3);
   //// cout << "s5: " << s5.toString() << endl;

   // // test subString
   // ConcatStringList s6 = s4.subString(2, 8);
   // cout << "s6: " << s6.toString() << endl;

   // // test reverse
   // ConcatStringList s7 = s4.reverse();
   // cout << "s7: " << s7.toString() << endl;

   /*ConcatStringList* s1 = new ConcatStringList("Hello");
    ConcatStringList* s2 = new ConcatStringList("_world");
    ConcatStringList* s3 = new ConcatStringList(s1->concat(*s2));

    cout << ConcatStringList::refList.refCountsString() << endl;
    cout << ConcatStringList::delStrList.totalRefCountsString() << endl;

    cout << "---After deleting s1---" << endl;
    delete s1;
    cout << ConcatStringList::refList.refCountsString() << endl;
    cout << ConcatStringList::delStrList.totalRefCountsString() << endl;

    cout << "---After deleting s3---" << endl;
    delete s3;
    cout << ConcatStringList::refList.refCountsString() << endl;
    cout << ConcatStringList::delStrList.totalRefCountsString() << endl;

    cout << "---After deleting s2---" << endl;
    delete s2;
    cout << ConcatStringList::refList.refCountsString() << endl;
    cout << ConcatStringList::delStrList.totalRefCountsString() << endl;*/

    /*ConcatStringList s1("Hello,");
    ConcatStringList s2("_this_is");
    ConcatStringList s3("_an_assignment");

    ConcatStringList s4 = s1.concat(s2);
    cout << "s4: " << s4.toString() << endl;*/
    
    ConcatStringList s0("RXOWl5pI");
    ConcatStringList s1("fWqBoqmXqoYzOk");
    ConcatStringList s2("");
    ConcatStringList s3("QSNZpFueMjdY");
    ConcatStringList s4("VfegY");
    ConcatStringList s5("pjb2crVf6N0Z6");
    ConcatStringList s6("RTlNU0VG49x");
    ConcatStringList s7("SdFTxUtsjpOET");
    ConcatStringList s8("9bh20");
    ConcatStringList s9("fLuz6n0vGaVG9");
    ConcatStringList s10("y4KKOrDH4QaYnVX6szH4");
    ConcatStringList s11("D9XsqKDeXrgdajw2Wbs");
    ConcatStringList s12("Bv9e33BHlQs7");
    ConcatStringList s13=s0.concat(s11);
    cout<<"s13: "<<s13.toString()<<endl;
     ConcatStringList s14=s4.concat(s7);
    cout<<"s14: "<<s14.toString()<<endl;
     ConcatStringList s15=s2.concat(s10);
    cout<<"s15: "<<s15.toString()<<endl;
     ConcatStringList s16=s8.concat(s6);
   cout<<"s16: "<<s16.toString()<<endl;
     //ConcatStringList s17=s15.subString(16,4);
   // cout<<"s17: "<<s17.toString()<<endl;
   // ConcatStringList s18=s1.subString(10,3);
   // cout<<"s18: "<<s18.toString()<<endl;
    /*ConcatStringList s19=s10.reverse();
    cout<<"s19: "<<s19.toString()<<endl;
    ConcatStringList s20=s7.reverse();
    cout<<"s20: "<<s20.toString()<<endl;
    ConcatStringList s21=s1.reverse();
    cout<<"s21: "<<s21.toString()<<endl;
    ConcatStringList s22=s0.reverse();
    cout<<"s22: "<<s22.toString()<<endl;*/
   cout<<"Rcount: "<<s16.refList.size()<<endl;
    return 0;
    /*
subString 17 15 16 4
subString 18 1 10 3
reverse 19 10
toString 19
reverse 20 7
toString 20
reverse 21 1
toString 21
reverse 22 0
toString 22
length refList
length delStrList
                */
            }
