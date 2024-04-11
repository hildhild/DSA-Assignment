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
        if (s[0]=='\0') node->CharArrayList = "";
        else node->CharArrayList=s;
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
            if (temp==tail) break;
           //if (i==count-1) break;
            temp = temp->next;
            //i++;
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
        if (from < 0 || from >= length() || to < 0 || to > length()) throw std::out_of_range("Index of string is invalid");
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
                if (to <= end) {
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
                        if (to <= end) {
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
            temp=temp->next;
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
        this->refList.Rsort();
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
                 //cout<<"vo while"<<endl;
                if (temp2->nodehead != nullptr && temp2->nodetail != nullptr) {
                    //cout<<"vo if 1"<<endl;
                    if (temp2->nodehead->node!=nullptr && temp2->nodetail->node!=nullptr){
                        if ((temp2->nodehead->numref + temp2->nodetail->numref) == 0 && temp2->nodehead->node->remove == 0 && temp2->nodetail->node->remove == 0 ) {
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
                            //temp4->CharArrayList=NULL;
                           // temp4->next=
                            //cout <<"xoa chuoi: "<< temp4->CharArrayList << endl;
                            if (temp4!=nullptr) 
                            
                            //
                            delete temp4;
                            temp4 = nullptr;
                        }

                        //cout <<"xoa chuoi: "<< temp3->CharArrayList << endl;
                        if (temp3!=nullptr)  
                       
                       // 
                       delete temp3;
                       temp3 = nullptr;
                       temp2->nodehead->node=nullptr;
                       temp2->nodetail->node=nullptr;
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
                        //cout << "xoa Dnode" << endl;
                        if (this->delStrList.Dcount == 0 && this->refList.sumref()==0) {
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
                    if (node== temp->node) {
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
                            break;
                           // temp=Rhead;
                            //continue;
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
                           //temp=prenode->Rnext;
                            //continue;
                            break;
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
                    bool change=0;
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
                                change=1;
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
                                    change=1;
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
                        if (change){
                            temp=temp1;
                            temp1=temp->Rnext;
                            change=0;
                        }
                        else
                        temp1 = temp1->Rnext;
                    }
                    if (change){
                       temp=temp1->Rnext;
                       change=0;
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
        void Rprint(){
            cout<<"\nPrint RNode: "<<endl;
            RNode* temp=Rhead;
            for (int i=0; i<this->Rcount; i++){
               cout<<temp->node->CharArrayList<<" "<<temp->numref<<endl;
               temp=temp->Rnext;
            }   
            cout<<endl;
            return;
        }
        int sumref(){
            int sum=0;
            RNode*temp=Rhead;
            while(temp!=NULL){
                sum+=temp->numref;
                temp=temp->Rnext;
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
ConcatStringList::ReferencesList ConcatStringList::refList = ConcatStringList::ReferencesList();
ConcatStringList::DeleteStringList ConcatStringList::delStrList = ConcatStringList::DeleteStringList();
int main() {
    /*ConcatStringList *s0=new ConcatStringList("xBobE2R2");
    ConcatStringList *s1=new ConcatStringList("kS");
    ConcatStringList *s2=new ConcatStringList("LeBr9");
    ConcatStringList *s3=new ConcatStringList(s0->concat(*s1));
//subString 4 2 4 4
//subString 5 0 7 1
ConcatStringList *s6=new ConcatStringList(s0->subString(2,8));
//subString 7 6 6 1
ConcatStringList *s8=new ConcatStringList(s6->subString(1, 3));
//subString 9 0 1 1
//subString 10 2 5 4
//subString 11 6 4 1
//subString 12 3 1 1
ConcatStringList *s13=new ConcatStringList(s6->subString(1, 5));
ConcatStringList *s14=new ConcatStringList(s6->reverse());
ConcatStringList *s15=new ConcatStringList(s1->reverse());
ConcatStringList *s16=new ConcatStringList(s13->reverse());
ConcatStringList *s17=new ConcatStringList(s15->reverse());
ConcatStringList *s18=new ConcatStringList(s13->reverse());
ConcatStringList *s19=new ConcatStringList(s16->reverse());
ConcatStringList *s20=new ConcatStringList(s0->reverse());
ConcatStringList *s21=new ConcatStringList(s6->reverse());
ConcatStringList *s22=new ConcatStringList(s0->reverse());
ConcatStringList *s23=new ConcatStringList(s17->reverse());
ConcatStringList *s24=new ConcatStringList(s14->reverse());
ConcatStringList *s25=new ConcatStringList(s17->reverse());
cout<<s25->refList.size()<<endl;
cout<<s25->delStrList.size()<<endl;
cout<<s25->refList.refCountsString()<<endl;
cout<<s25->delStrList.totalRefCountsString()<<endl;
delete s25;
delete s24;
delete s23;
delete s22;
delete s21;
delete s20;
delete s19;
delete s18;
delete s17;
delete s16;
delete s15;
delete s14;
delete s13;
delete s8;
delete s6;
delete s3;
delete s2;
delete s1;
cout<<"del s1: "<<endl;
cout<<s0->refList.refCountsString()<<endl;
cout<<s0->delStrList.totalRefCountsString()<<endl;
delete s0;
cout<<"del s0: "<<endl;
cout<<s0->refList.refCountsString()<<endl;
cout<<s0->delStrList.totalRefCountsString()<<endl;*/

    /*ConcatStringList s0("b9OoqWBCmi1CkuzQwFY");
    ConcatStringList s1("xj2hQrCqPS");
    ConcatStringList s2("");
    ConcatStringList s3("Hi4");
    ConcatStringList s4("Q1a6");
    ConcatStringList s5("gZuKvaYeG8H67Yc3Cta6");
    ConcatStringList s6("");
    ConcatStringList s7("dg71psIa06");
    ConcatStringList s8("hjT0RIlKS5");
    ConcatStringList s9("7yJIA");
    ConcatStringList *s10=new ConcatStringList(s9.concat(s1));
    ConcatStringList *s11=new ConcatStringList(s8.concat(s2));
    ConcatStringList *s12=new ConcatStringList(s6.concat(s0));
    ConcatStringList *s13=new ConcatStringList(s7.concat(s4));
    ConcatStringList *s14=new ConcatStringList(s3.concat(s5));
    //ConcatStringList *s15=new ConcatStringList(s6.subString(0,0));
    //ConcatStringList *s16=new ConcatStringList(s14->subString(23,14));
    //ConcatStringList *s17=new ConcatStringList(s14->subString(21,2));
    ConcatStringList *s18=new ConcatStringList(s11->subString(1,5));
    ConcatStringList *s19=new ConcatStringList(s7.subString(3,10));
    //ConcatStringList *s20=new ConcatStringList(s8.subString(3,1));
    //ConcatStringList *s21=new ConcatStringList(s10->subString(2,2));
    ConcatStringList *s22=new ConcatStringList(s12->subString(5,10));
    ConcatStringList *s23=new ConcatStringList(s14->subString(18,23));
    ConcatStringList *s24=new ConcatStringList(s14->subString(6,20));
    ConcatStringList *s25=new ConcatStringList(s1.subString(5,9));
    ConcatStringList *s26=new ConcatStringList(s19->subString(1,6));
    ConcatStringList *s27=new ConcatStringList(s6.reverse());
    ConcatStringList *s28=new ConcatStringList(s6.reverse());
    ConcatStringList *s29=new ConcatStringList(s8.reverse());
    ConcatStringList *s30=new ConcatStringList(s13->reverse());
    ConcatStringList *s31=new ConcatStringList(s1.reverse());
ConcatStringList *s32=new ConcatStringList(s25->reverse());
ConcatStringList *s33=new ConcatStringList(s28->reverse());
ConcatStringList *s34=new ConcatStringList(s27->reverse());
ConcatStringList *s35=new ConcatStringList(s9.reverse());
ConcatStringList *s36=new ConcatStringList(s11->reverse());
ConcatStringList *s37=new ConcatStringList(s32->reverse());
ConcatStringList *s38=new ConcatStringList(s34->reverse());
ConcatStringList *s39=new ConcatStringList(s2.reverse());
ConcatStringList *s40=new ConcatStringList(s23->reverse());
ConcatStringList *s41=new ConcatStringList(s37->reverse());
ConcatStringList *s42=new ConcatStringList(s30->reverse());
ConcatStringList *s43=new ConcatStringList(s32->reverse());
    cout<<s43->refList.refCountsString()<<endl;
    cout<<s43->delStrList.totalRefCountsString()<<endl;
    //s22->refList.Rprint();
    cout<<s43->refList.size()<<endl;
    delete s43;
    cout<<"del s43"<<endl;
    //s21->refList.Rprint();
    cout<<s42->refList.refCountsString()<<endl;
    cout<<s42->delStrList.totalRefCountsString()<<endl;
    delete s42;
    cout<<"del s42"<<endl;
    cout<<s41->refList.refCountsString()<<endl;
    cout<<s41->delStrList.totalRefCountsString()<<endl;
    delete s40;
    cout<<"del s40"<<endl;
    cout<<s39->refList.refCountsString()<<endl;
    cout<<s39->delStrList.totalRefCountsString()<<endl;
    delete s39;
    cout<<"del s39"<<endl;
    cout<<s38->refList.refCountsString()<<endl;
    cout<<s38->delStrList.totalRefCountsString()<<endl;
   //s16.refList.Rprint();*/

/*ConcatStringList s0("RXOWl5pI");
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
    //s11.refList.Rprint();
    ConcatStringList s12("Bv9e33BHlQs7");
    ConcatStringList s13=s0.concat(s11);
    //s13.refList.Rprint();
    cout<<"s13: "<<s13.toString()<<endl;
     ConcatStringList s14=s4.concat(s7);
     //s14.refList.Rprint();
    cout<<"s14: "<<s14.toString()<<endl;
    ConcatStringList s15=s2.concat(s10);
    cout<<"s15: "<<s15.toString()<<endl;
     ConcatStringList s16=s8.concat(s6);
   cout<<"s16: "<<s16.toString()<<endl;
    ConcatStringList *s19=new ConcatStringList(s10.reverse());
    cout<<"s19: "<<s19->toString()<<endl;
    ConcatStringList *s20=new ConcatStringList(s7.reverse());
    cout<<"s20: "<<s20->toString()<<endl;
    ConcatStringList *s21=new ConcatStringList(s1.reverse());
    cout<<"s21: "<<s21->toString()<<endl;
    ConcatStringList *s22=new ConcatStringList(s0.reverse());
    cout<<"s22: "<<s22->toString()<<endl;
    cout<<s22->refList.refCountsString()<<endl;
    cout<<s22->delStrList.totalRefCountsString()<<endl;
    //s22->refList.Rprint();
    delete s22;
    cout<<"del s22"<<endl;
    //s21->refList.Rprint();
    cout<<s21->refList.refCountsString()<<endl;
    cout<<s21->delStrList.totalRefCountsString()<<endl;
    delete s21;
    cout<<"del s21"<<endl;
    cout<<s20->refList.refCountsString()<<endl;
    cout<<s20->delStrList.totalRefCountsString()<<endl;
    delete s20;
    cout<<"del s20"<<endl;
    cout<<s19->refList.refCountsString()<<endl;
    cout<<s19->delStrList.totalRefCountsString()<<endl;
    delete s19;
    cout<<"del s19"<<endl;
    cout<<s15.refList.refCountsString()<<endl;
    cout<<s15.delStrList.totalRefCountsString()<<endl;*/
   //s16.refList.Rprint();
return 0;

}
