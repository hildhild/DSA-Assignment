#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"

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
        if (s[0] == '\0') node->CharArrayList = "";
        else node->CharArrayList = s;
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
        //int i=0;
        while (temp != NULL) {
            s += temp->CharArrayList;
            if (temp == tail) break;
            //if (i==count-1) break;
            temp = temp->next;
            //i++;
        }
        //delete temp;
        return s;
    }
    ConcatStringList concat(const ConcatStringList& otherS) const { //O(1)
        ConcatStringList snew = ConcatStringList();
        /* CharALNode* temp = head;
         while (temp != NULL) {
             snew->add(temp->CharArrayList);
             temp = temp->next;
         }*/
        snew.head = this->head;
        this->tail->next = otherS.head;
        snew.tail = otherS.tail;
        snew.count = this->count + otherS.count;
        snew.size = this->size + otherS.size;
        //Ref
        this->refList.addRnode(snew.head);
        this->refList.addRnode(snew.tail);
        return snew;
    }
    ConcatStringList subString(int from, int to) const {
        if (from < 0 || from >= length() || to < 0 || to > length()) throw std::out_of_range("Index of string is invalid");
        else if (from >= to) throw std::logic_error("Invalid range");
        ConcatStringList snew = ConcatStringList();
        CharALNode* temp = head;
        while (temp != tail) {
            snew.add(temp->CharArrayList);
            temp = temp->next;
        }
        snew.add(temp->CharArrayList);
        CharALNode* oldhead = snew.head;
        CharALNode* oldtail = snew.tail;
        int oldsize = snew.size;
        int end = 0;
        int start = 0;
        int delete_head_count = 0;
        temp = snew.head;
        while (temp != NULL) {
            //for (int i = 0; i < count; i++) {
            end += temp->CharArrayList.length();
            if (from < end) {
                snew.head = temp;
                if (to <= end) {
                    snew.tail = temp;
                    snew.count = 1;
                    temp->CharArrayList = temp->CharArrayList.substr(from - start, to - from);
                    //snew.size = temp->CharArrayList.length();
                    snew.size = to - from;
                    //Ref
                    this->refList.addRnode(snew.head);
                    this->refList.addRnode(snew.tail);
                    //xoa cac node cuoi
                    if (end < oldsize) {
                        /*if(temp->next != oldtail){
                            CharALNode* temp_delete = temp->next;
                            while(temp_delete!=oldtail){
                                temp = temp_delete->next;
                                delete temp_delete;
                            }
                        }*/
                        if (temp != oldtail) {
                            CharALNode* temp_delete = temp->next;
                            while (temp_delete != oldtail->next) {
                                temp = temp_delete->next;
                                delete temp_delete;
                                temp_delete = temp;
                            }
                        }
                    }
                    //Xoa delete_head_tail node dau tien
                    /*CharALNode* temp_delete = oldhead;
                    for(int i=0; i<delete_head_count; i++){
                        oldhead = temp_delete->next;
                        delete temp_delete;
                        temp_delete = oldhead;
                    }*/
                    CharALNode* temp_delete = oldhead;
                    CharALNode* pre_delete = temp_delete;
                    for (int i = 0; i < delete_head_count; i++) {
                        temp_delete = temp_delete->next;
                        delete pre_delete;
                        pre_delete = temp_delete;
                    }
                    return snew;
                }
                else {
                    int l = temp->CharArrayList.length();
                    temp->CharArrayList = temp->CharArrayList.substr(from - start, end);
                    //snew.size += temp->CharArrayList.length();
                    start += l;
                    temp = temp->next;
                    snew.count = 2;
                    end += temp->CharArrayList.length();
                    while (temp != NULL) {
                        if (to <= end) {
                            temp->CharArrayList = temp->CharArrayList.substr(0, to - start);
                            //snew.size += temp->CharArrayList.length();
                            snew.tail = temp;
                            snew.size = to - from;
                            //Ref
                            this->refList.addRnode(snew.head);
                            this->refList.addRnode(snew.tail);
                            //
                            // ConcatStringList res = *snew;
                            // delete snew;
                            // return res;
                            //xóa cac node cuoi
                            if (end < oldsize) {
                                /*if(temp->next != oldtail){
                                    CharALNode* temp_delete = temp->next;
                                    while(temp_delete!=oldtail){
                                        temp = temp_delete->next;
                                        delete temp_delete;
                                    }
                                }*/
                                if (temp != oldtail) {
                                    CharALNode* temp_delete = temp->next;
                                    while (temp_delete != oldtail->next) {
                                        temp = temp_delete->next;
                                        delete temp_delete;
                                        temp_delete = temp;
                                    }
                                }
                            }
                            //Xoa delete_head_tail node dau tien
                            /*CharALNode* temp_delete = oldhead;
                            for(int i=0; i<delete_head_count; i++){
                                oldhead = temp_delete->next;
                                delete temp_delete;
                                temp_delete = oldhead;
                            }*/
                            CharALNode* temp_delete = oldhead;
                            CharALNode* pre_delete = temp_delete;
                            for (int i = 0; i < delete_head_count; i++) {
                                temp_delete = temp_delete->next;
                                delete pre_delete;
                                pre_delete = temp_delete;
                            }
                            return snew;
                        }
                        start += temp->CharArrayList.length();
                        snew.count++;
                        snew.size += temp->CharArrayList.length();
                        temp = temp->next;
                        end += temp->CharArrayList.length();
                    }
                }
            }
            else {
                delete_head_count++;
                start += temp->CharArrayList.length();
                temp = temp->next;
            }
        }
        return snew;
    }
    ConcatStringList reverse() const {
        ConcatStringList snew = ConcatStringList();
        CharALNode* temp = head;
        while (temp != NULL) {
            snew.add(temp->CharArrayList);
            if (temp == tail) break;
            temp = temp->next;
        }
        temp = snew.head;
        CharALNode* prev = NULL;
        CharALNode* next = NULL;
        while (temp != NULL) {
            next = temp->next;
            temp->next = prev;
            prev = temp;
            temp = next;
        }

        snew.head = prev;
        //delete prev;
        //delete next;
        snew.count = this->count;
        temp = snew.head;
        int i = 0;
        while (temp != NULL) {
            if (i == snew.count - 1) {
                snew.tail = temp;
                break;
            }
            i++;
            temp = temp->next;
        }
        temp = snew.head;
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
        this->refList.addRnode(snew.head);
        this->refList.addRnode(snew.tail);
        return snew;
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
        //this->refList.Rsort();
        RNode* temp1 = this->refList.Rhead;
        while (temp1 != NULL) {
            if (temp1->node == this->tail) {
                temp1->numref--;
                break;
            }
            temp1 = temp1->Rnext;
        }
        this->refList.Rsort();
        //cout<<this->refList.refCountsString()<<endl;
        this->delStrList.addDnode(temp, temp1);
        //cout<<"Dcount: "<<this->delStrList.Dcount<<endl;
        //cout<<this->delStrList.totalRefCountsString()<<endl;
        if (this->delStrList.Dcount > 0) {
            DNode* temp2 = this->delStrList.Dhead;
            while (temp2 != NULL) {
                // cout<<"vo while"<<endl;
                if (temp2->nodehead != nullptr && temp2->nodetail != nullptr) {
                    // cout<<"vo if 1"<<endl;
                    if (temp2->nodehead->node != nullptr && temp2->nodetail->node != nullptr) {
                        if ((temp2->nodehead->numref + temp2->nodetail->numref) == 0 && temp2->nodehead->node->remove == 0 && temp2->nodetail->node->remove == 0) {
                            //  cout<<"vo if 2"<<endl;
                              //cout << temp2->nodehead->node->CharArrayList<< endl;
                            CharALNode* temp3 = temp2->nodehead->node;
                            CharALNode* temp4;
                            while (temp3 != temp2->nodetail->node) {
                                //cout << temp3->CharArrayList << endl;
                                temp4 = temp3;
                                temp4->remove = 1;
                                temp3 = temp3->next;
                                //temp3->remove = 1;
                                //temp4->CharArrayList=NULL;
                               // temp4->next=
                                //cout <<"xoa chuoi: "<< temp4->CharArrayList << endl;
                                if (temp4 != nullptr)

                                    //
                                    delete temp4;
                                temp4 = nullptr;
                            }

                            //cout <<"xoa chuoi: "<< temp3->CharArrayList << endl;
                            if (temp3 != nullptr)

                                // 
                                delete temp3;
                            temp3 = nullptr;
                            temp2->nodehead->node = nullptr;
                            temp2->nodetail->node = nullptr;
                            // this->count=0;
                            // this->size=0;
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
                        // cout << "xoa Dnode" << endl;
                        if (this->delStrList.Dcount == 0 && this->refList.sumref() == 0) {
                            //cout << "xoa Ref" << endl;
                            this->refList.removeallRnode();
                        }
                        //cout << "temp2: " << temp2->nodehead->node->CharArrayList << endl;
                        //if (temp2->nodehead->node != nullptr) cout<<"null"<<endl;
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
                    //cout<<temp->numref<<" ";
                    //str += temp->node->CharArrayList;
                    str += ",";
                    temp = temp->Rnext;
                }
                str += (char)temp->numref + 48;
                //cout<<temp->numref<<endl;
                //str += temp->node->CharArrayList;
            }
            str += "]";
            return str;
        }
        void addRnode(CharALNode* node) {

            // if (Rhead!=NULL) cout<<"Rhead: "<<this->Rhead->node->CharArrayList<<endl;
             //cout<<"Rcount: "<<Rcount<<endl;
            if (this->Rcount == 0) {

                RNode* newnode = new RNode();
                newnode->node = node;
                newnode->numref = 1;
                newnode->Rnext = NULL;
                this->Rhead = this->Rtail = newnode;
                // cout<<"tao moi count=0: "<<newnode->node->CharArrayList<<endl;
                this->Rcount++;
                return;
            }
            else {
                RNode* temp = this->Rhead;
                while (temp != NULL) {
                    if (node == temp->node) {
                        //if (node->CharArrayList== temp->node->CharArrayList) {
                            //cout<<"co giong "<<temp->node->CharArrayList<<endl;
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
                //cout<<"tao moi "<<temp1->node->CharArrayList<<endl;
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
                            temp = Rhead;
                        }
                        else {
                            RNode* prenode = Rhead;
                            while (prenode->Rnext != temp) prenode = prenode->Rnext;
                            prenode->Rnext = temp->Rnext;
                            Rtail->Rnext = temp;
                            temp->Rnext = NULL;
                            Rtail = temp;
                            temp = prenode->Rnext;
                        }
                    }
                    else {
                        temp = temp->Rnext;
                    }
                }
                temp = Rhead;
                for (int i = 0; i < Rcount - 1 && temp != nullptr; i++) {
                    bool change = 0;
                    RNode* temp1 = temp->Rnext;
                    for (int j = i + 1; j < Rcount && temp1 != nullptr; j++) {
                        if (temp1->numref < temp->numref && temp1->numref != 0 && temp->numref != 0) {
                            if (Rcount == 2) {
                                temp1->Rnext = temp;
                                temp->Rnext = NULL;
                                Rhead = temp1;
                                Rtail = temp;
                                return;
                            }
                            else if (temp == Rhead) {
                                RNode* postnode = temp1->Rnext;
                                temp->Rnext = postnode;
                                temp1->Rnext = temp;
                                Rhead = temp1;
                                change = 1;
                            }
                            else {
                                RNode* prenode = Rhead;
                                for (int k = 0; k < i - 1; k++) {
                                    prenode = prenode->Rnext;
                                }
                                if (j <= Rcount - 2) {
                                    RNode* postnode = temp1->Rnext;
                                    prenode->Rnext = temp1;
                                    temp1->Rnext = temp;
                                    temp->Rnext = postnode;
                                    change = 1;
                                }
                                else {
                                    prenode->Rnext = temp1;
                                    temp1->Rnext = temp;
                                    temp->Rnext = NULL;
                                    Rtail = temp;
                                    return;
                                }
                            }

                            /*CharALNode* tempnode = temp->node;
                            int tempnumref = temp->numref;
                            temp->node = temp1->node;
                            temp->numref = temp1->numref;
                            temp1->node = tempnode;
                            temp1->numref = tempnumref;*/
                        }
                        if (change) {
                            temp = temp1;
                            temp1 = temp->Rnext;
                            change = 0;
                        }
                        else
                            temp1 = temp1->Rnext;
                    }
                    if (change) {
                        temp = temp1->Rnext;
                        change = 0;
                    }
                    else temp = temp->Rnext;
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
        void Rprint() {
            cout << "\nPrint RNode: " << endl;
            RNode* temp = Rhead;
            for (int i = 0; i < this->Rcount; i++) {
                cout << temp->node->CharArrayList << " " << temp->numref << endl;
                temp = temp->Rnext;
            }
            cout << endl;
            return;
        }
        int sumref() {
            int sum = 0;
            RNode* temp = Rhead;
            while (temp != NULL) {
                sum += temp->numref;
                temp = temp->Rnext;
            }
            return sum;
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


#endif // __CONCAT_STRING_LIST_H__