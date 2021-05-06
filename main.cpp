#include <iostream>
#include <vector>
#include <string> 
#include <chrono>
#include <cmath>
#define DEBUG false

int ido = 0;
long long nodeCounter = 0;
long long NRnodeCounter = 0;


struct TtrieNode;

struct SufLink {
    int ind;
    TtrieNode* Link;

    SufLink() {
        ind = -1;
        Link = nullptr;
    }

    SufLink(int in, TtrieNode* lin) {
        ind = in;
        Link = lin;
    }
};


struct TtrieNode {
    int beg;
    int end;
    int* endl;

    std::string text;
    std::vector<TtrieNode*> children;

    TtrieNode* parent;

    SufLink SL;
    int id;

    int len;

    TtrieNode (std::string bod, TtrieNode* par, int* endInd) {
        beg = 0;
        end = 0;

        endl = endInd;

        text = bod;
        children = std::vector<TtrieNode*>(0);
        parent = par;

        SL.Link = nullptr;
        SL.ind = -1;
        id = ido;
        ido++;
        len = 0;
    }

    TtrieNode(TtrieNode* par, int* endInd) {
        beg = 0;
        end = 0;

        endl = endInd;

        text = std::string();
        children = std::vector<TtrieNode*>(0);
        parent = par;

        SL.Link = nullptr;
        SL.ind = -1;
        id = ido;
        ido++;
        len = 0;
    }

    ~TtrieNode() {
        for (int i = 0; i < children.size(); ++i) {
            delete children[i];
        }
    }

    bool List() {
        return children.size() == 0 ? true : false;
    }

    int End() {
        if (List()) {
            return *endl;
        }
        else {
            return end;
        }
    }
 
    int Len() {
        len = (parent != nullptr ? parent->len : 0) + (End() - beg) + 1;

        return len;
    }

    int SelfLen() {
        return (End() - beg) + 1;
    }
 
    TtrieNode* Deseparate(int pos) {
        //���������� ���� �� ������� pos � ��� ������
        //����� ������ �� ������� (������������) �������� � ����� ����
        //������ ���� �������� �������� ������, � ��� ����������� ������, � ��� ���� � ������ ���� ������

        if (DEBUG)printf("sep: %d |%d| %d\n", beg, pos, End());

        if (pos == End()) {
            return this;
        }

        int ps = -1;
        if (parent != nullptr) {
            ps = parent->childInd(text[beg]);
        }

        TtrieNode* Past = new TtrieNode(parent, endl);
        Past->beg = beg;
        Past->end = Past->beg + pos;
        beg = Past->end + 1;
        Past->text = text;


        Past->len = (Past->end - Past->beg) + 1;
        if(parent != nullptr)
        Past->len += parent->len;
        len = (End() - beg) + Past->len + 1;


        if (ps != -1) {
            parent->children[ps] = Past;
        }
        parent = Past;

        Past->children.push_back(this);
        

        return Past;
    }

    bool IsList() {
        if (children.size() == 0) {
            return true;
        }
        else {
            return false;
        }
    }

    void Prolong (char a) {
        text.push_back(a);
        end++;
    }

    void AddChild (TtrieNode* ch) {
        children.push_back(ch);
    }

    void AddChild(const std::string& chS) {
        try {
            TtrieNode* Past = new TtrieNode(chS, this, endl);
            Past->beg = end + 1;
            Past->end = end + 1;

            children.push_back(Past);
        }
        catch (std::bad_alloc a) {
            throw - 1;
        }
    }

    void AddChild(int left, int right) { //������ ������� � ���������
        try {
            TtrieNode* Past = new TtrieNode(this, endl);
            Past->text = text;
            Past->beg = left;
            Past->end = right;
            Past->len = -1;

            children.push_back(Past);
        }
        catch (std::bad_alloc a) {
            throw - 1;
        }
    }

    TtrieNode* child(char firstLetter) {
        //���������� ��������� �� �������, ��� ������ ���������� � ����� firstLetter
        //�������� ������ ���� ��� ��������� ��������

        for (TtrieNode* iter : children) {
            if (iter->text[iter->beg] == firstLetter) {
                return iter;
            }
        }

        return nullptr;
    }

    int childInd(char firstLetter) {
        //���������� ��������� �� �������, ��� ������ ���������� � ����� firstLetter
        //�������� ������ ���� ��� ��������� ��������

        for (int i = 0; i < children.size(); ++i) {
            if (children[i]->text[children[i]->beg] == firstLetter) {
                return i;
            }
        }

        return -1;
    }

    void SLadd(TtrieNode* linkTo, int tm) {
        SL.Link = linkTo;
        SL.ind = tm;
    }

    std::vector<TtrieNode*> Children() {
        return children;
    }

    std::string Str() {
        return text;
    }

    char sInd(int i) {
        if (beg + i <= End()) {
            return text[beg + i];
        }
        else {
            return 0;
        }
    }
};

std::string suffix(std::string line, int s) {
    //������� � s �� �����

    std::string ret;
    for (int i = s; i < line.size(); ++i) {
        ret.push_back(line[i]);
    }
    return ret;
}

void PrintTrie(TtrieNode* root, int depth) {
    for (int i = 0; i < depth; ++i) {
        printf("\t");
    }
    //std::cout << root->text;
    if (root->List()) {
        printf("!");
        for (int i = root->beg; i <= (root->endl == nullptr ? -1 : *(root->endl)); ++i) {
            printf("%c", root->text[i]);
        }
        printf("!");
        printf("%d_%d| id: %d (len: %d|%d)", root->beg, root->End(), root->id, root->Len(), root->SelfLen());
    }
    else {
        printf("!");
        for (int i = root->beg; i <= root->end; ++i) {
            printf("%c", root->text[i]);
        }
        printf("!");
        printf("%d_%d| id: %d (len: %d|%d)", root->beg, root->End(), root->id, root->Len(), root->SelfLen());
    }
    printf(" [");
    if (root->SL.Link != nullptr)std::cout << "(" << root->SL.Link->id << " <-id)|" << root->SL.Link->beg << "_" << root->SL.Link->End();
    else printf("NULL");
    printf("]\n");
    for (int i = 0; i < root->children.size(); ++i) {
        PrintTrie(root->children[i], depth + 1);
    }
}

struct TstartPos {
    int Ind;
    int TextSrartPos;
    TtrieNode* Link;
    TtrieNode* FromSLtoPOINT;

    TstartPos() {
        Ind = -1;
        Link = nullptr; 
        TextSrartPos = -1;
        FromSLtoPOINT = nullptr;
    }

    TstartPos(int ind, TtrieNode* link) {
        Ind = ind;
        Link = link;
        TextSrartPos = 0;
        FromSLtoPOINT = nullptr;
    }

    TstartPos(int ind, TtrieNode* link, TtrieNode* fromSLtoPOINT) {
        Ind = ind;
        Link = link;
        TextSrartPos = 0;
        FromSLtoPOINT = fromSLtoPOINT;
    }
};

TstartPos prolongate(TtrieNode* root, TtrieNode* start, int term /*������� � ������� �� �������*/, std::string text, int rightBD, TtrieNode* fromSLtoPOINT /*����, � ������� ������� ������*/) {
    
    if(DEBUG)printf("approch node: %d term: %d / %d\n", start->id, term, rightBD);

    while (start != root && start->Len() - start->SelfLen() >= term) {
        start = start->parent;
    }

    term -= (start->Len() - start->SelfLen());

    if (DEBUG)printf("passed to node: %d term: %d / %d\n", start->id, term, rightBD);

    //start - ����, �� ������� ���� ���������
    //term - ������� � ���
    //fromSLtoPOINT - �� ����� ���. ������ ����� ������ � ������� ��� ���������


    //������� �� ������� ����������� ��������� �� ����. ����.
    //�������: ����� �� ��������?
    //���� �����, �������, �������. �� ����. ���
    //���� ������, ��������� ������, �������� �� ���. �������, ���� �� ����� �����.

    ++nodeCounter;

    //�������� ���������� ��������� � ������
    if (start == root) {
        if (term <= 0) {
            TtrieNode* ch = root->child(text[rightBD]);

            if (ch == nullptr) { //�������� �������� �� ���������, ���� ���. �������
                try {
                    root->AddChild(rightBD, rightBD);
                }
                catch (int a) {
                    throw - 1;
                }

                TtrieNode* aded = root->child(text[rightBD]);
                aded->SL.Link = root;
                aded->SL.ind = -1;

                //����������� ���������� ������ 
                if (fromSLtoPOINT != nullptr) {
                    fromSLtoPOINT->SL.Link = aded;
                    fromSLtoPOINT->SL.ind = -1;
                }

                return TstartPos(-1 /*� ����� ������ � �������� �� ��������� ������� ��������*/, ch, aded); //������ ��������� �� �������, �� ���. ������ �������� ����� ������� � ������
            }
            else {
                //����� �������� ��������?
                if (fromSLtoPOINT != nullptr) {
                    fromSLtoPOINT->SL.Link = ch;
                    fromSLtoPOINT->SL.ind = -1;
                }


                return  TstartPos(0, ch, fromSLtoPOINT); //�� 0 ��-�� ���� �� ���. ������ �� fromSLtoPOINT ��� ��������, � ���. ��� ��������� �� 1-��
            }
        }
        else {
            TtrieNode* ch = root->child(text[term]);
            return prolongate(root, /*���� ����*/ ch, term, /*� ��� ����*/ text, rightBD, /*������ ����*/ fromSLtoPOINT);
        }

        

    }
    //������ �� � ������
    
    ++NRnodeCounter;

    //������ ��������� �� ������� � ������ ����
    //1. ������ � ����� ����� (-1)

    if (term == -1) {
        printf("ERROR!\n");
        return TstartPos(-1, nullptr);
    }

    //2. ������ �� ���������� ����

    else {
        //���� ����������� ��� �� ������ � ��������, ������������� �� �����


        if (start->End() < start->beg + term) {
            //���� �� � ����� �������, ������ ������� ����

            //��� ������ ������ �� �����

            TtrieNode* ch = start->child(text[rightBD]);
            if (ch == nullptr) {  //�������� �������� ������, ��������� �������
                try {
                    start->AddChild(*(start->endl), *(start->endl));
                }
                catch (int a) {
                    throw - 1;
                }

                TtrieNode* aded = start->child(text[rightBD]);


                //����������� ���������� ������ 
                if (fromSLtoPOINT != nullptr) {
                    fromSLtoPOINT->SL.Link = aded;
                    fromSLtoPOINT->SL.ind = -1;
                }


                if (start->SL.Link != nullptr) {
                    return prolongate(root, /*���� ����*/ start->SL.Link, (aded->Len() - aded->SelfLen()) - 1, /*� ��� ����*/ text, rightBD, /*������ ����*/ aded);
                }
                return TstartPos(-1, nullptr); //�� ���� �� �������� �� ����� �� ������ �������
            }
            else {
                //������� �������� �� ��� ��������� ����
                //���� ���� ����� 1, �� �������� �� �����, �� �� ��� ����� ��� ������ ������ ��� ��� ����
                //�� ������ ������

                //����������� ���������� ������ 
                if (fromSLtoPOINT != nullptr) {
                    fromSLtoPOINT->SL.Link = ch;
                    fromSLtoPOINT->SL.ind = -1;
                }

                return  TstartPos(0 + (ch->Len() - ch->SelfLen()), ch, fromSLtoPOINT);
            }


            
        }
        else
        if (start->End() >= term && start->text[start->beg + term] == text[rightBD]) {
            //���� ����� �������� �������� ���������, �� ������ �� ������
            
            return TstartPos(term + (start->Len() - start->SelfLen()), start, fromSLtoPOINT);
        }
        else {
            //���� ����. ����� ������ ���� ������� �� ����� � ���. ������, �� ����� ���������� ����������

            //������ ��������, ��� �� �� � ����� ������ � �� ����� ���������� �������� �� �������
            TtrieNode* ssL = start->SL.Link;
            TtrieNode* past = start->Deseparate(term - 1); 
            //��� ����������� ������ ����������� ��������� ����� �� ����� ���������� ������ ������������ start, � ������ ���������� ������ start ����� ���������� ������� next
            //� next ����������� ����������� ����
            // |----START----|
            // |-NEXT-||START|

            try {
                past->AddChild(*(past->endl), *(past->endl));
            }
            catch (int a) {
                throw - 1;
            }

            TtrieNode* aded = past->child(text[rightBD]);
            past->SL.Link = root; //���� �� ���� ���� ���� �������� ��������, �� ������ ����������� �����, ����� root � �����
            past->SL.ind = -1;

            //������������� ������ (�� �����������)
            if (fromSLtoPOINT != nullptr) {
                if (fromSLtoPOINT->parent != root) {
                    //���� � �� ���������� ���� ���� �������, �� ���� ����������� � ������ ��������
                    fromSLtoPOINT->parent->SL.Link = past;
                }

                fromSLtoPOINT->SL.Link = aded;
                fromSLtoPOINT->SL.ind = -1;
            }

            //���� ������
            if (ssL != nullptr) {

                return prolongate(root, /*���� ����*/ ssL  , term + (past->Len() - past->SelfLen()) - 1,/*� ��� ����*/ text, rightBD, /*������ ����*/ aded);
                //�� ����. ���� ���� ���������� ���. ������ �� ���. ��.
                //� ���� ����� ������, �� ������� �� ������������, �����������, �� ����������
            }
            return TstartPos(-1, nullptr);//��������� ������ ������� �����
        }
    }

}

TstartPos startAdd(TtrieNode* root, std::string text, int end, TtrieNode* lastEnd, TstartPos toSt) {
    //������� ����������� ���������� �������� � ������ �� �����
    TtrieNode* st = root;
    int stg = -1;

    if (toSt.Link == nullptr) {
        st = root;
        stg = -1;
    }
    else {
        st = toSt.Link;
        stg = toSt.Ind + 1;
    }

    try {
        toSt = prolongate(root, st, stg, text, end, toSt.FromSLtoPOINT);
    }
    catch (int a) {
        throw - 1;
    }

    if (DEBUG) {
        if (toSt.Link != nullptr) printf("|first cont| id: %d, ind: %d, linked: %d\n", toSt.Link->id, toSt.Ind, toSt.FromSLtoPOINT != nullptr ? toSt.FromSLtoPOINT->id : -1);
        else printf("non cont. sl returned: %d\n", toSt.FromSLtoPOINT != nullptr ? toSt.FromSLtoPOINT->id : -1);
    }

    return toSt;
}


class TsuffixTrie {
    //������ ���������� ���
private:
    TtrieNode* root;

    TtrieNode* lastNon;
    int* endID;
    TstartPos toSt;
public:

    TsuffixTrie(std::string text) {
        endID = new int(-1);

        root = new TtrieNode("|ROOT|", nullptr, endID);
        root->len = 0;
        root->end = -1;
        root->text = text;
        lastNon = root;
        toSt = TstartPos(-1, nullptr);

    }

    ~TsuffixTrie() {
        delete root;
    }

    TtrieNode* Root() {
        return root;
    }

    void SetRoot(TtrieNode* newRoot) {
        root = newRoot;
    }

    int Add(std::string text, int end) {
        (*endID)++; //��� �� � ���� �������� ��������� ���� ��� � �������������??
        try {
            toSt = startAdd(root, text, end, lastNon, toSt);
        }
        catch (int a) {
            return - 1;
        }

        return 0; 
    }

    void Print() {
        std::cout << "TEXT: " << root->text << "\nTRIE:\n";
        PrintTrie(root, 0);
    }

    std::vector<int> concat(std::vector<int> lhs, std::vector<int> rhs) {
        for (int i = 0; i < rhs.size(); ++i) {
            lhs.push_back(rhs[i]);
        }
        return lhs;
    }

    std::vector<int> takeAllEnds(TtrieNode* start) {
        if(DEBUG) printf("collecting in node %d\n", start->id);
        std::vector<int> res(0);
        std::vector<TtrieNode*> next = start->Children();
        if (next.size() == 0) {
            return std::vector<int>(1, start->text.size() - start->Len());
        }
        else {
            for (int i = 0; i < next.size(); ++i) {
                res = concat(res, takeAllEnds(next[i]));
            }
            return res;
        }
    }

    std::vector<int> locate(std::string exp) {
        TtrieNode* cur = root->child(exp[0]);

        if (cur == nullptr) {
            if (DEBUG) printf("location error: no start point\n");
            return std::vector<int>();
        }

        int pos = 0;
        for (int i = 0; i < exp.size(); ++i) {
            if (cur->sInd(pos) == 0) {

                if (cur->child(exp[i]) == nullptr) {
                    if (DEBUG) printf("location error: no cont point (node: %d | pos in exmp %d|%d <-pos in node)\n", cur->id, i, pos);
                    return std::vector<int>();
                }
                else {
                    cur = cur->child(exp[i]);
                    pos = 0;
                }
            }


            if (cur->sInd(pos) != exp[i]) {
                if(DEBUG) printf("location error: in-node missmatch (node id: %d| pos in exmmp-> %d|%c|<-exmp|trie->|%c|%d <-pos in node)\n", cur->id, i, exp[i], cur->sInd(pos), pos);
                return std::vector<int>();
            }

            ++pos;
        }

        return takeAllEnds(cur);
    }
};

int rad(int digit, int pos) {
    int del = pow(10, pos);
    int dif = pow(10, pos - 1);

    if (digit % dif == digit) {
        return -1;
    }

    int res = (digit % del) / dif;
    return res;
}

std::vector<int> radixSort(std::vector<int> input) { //������, ������� ��������
    std::vector<int> prom(input.size());
    int countArr[11]; 
    for (int i = 0; i < 11; ++i)
        countArr[i] = 0;

    int digit = 1;

    while (countArr[0] != input.size()) {


        for (int i = 0; i < 11; ++i)
            countArr[i] = 0;
        
        for (int i = 0; i < input.size(); ++i) {
            countArr[rad(input[i], digit) != -1 ? rad(input[i], digit) + 1 : 0] += 1;
        }

        int k = 0;
        int flp = 0;
        for (int i = 0; i < 11; ++i) {
            k = countArr[i];
            countArr[i] = flp;
            flp = k;
        }
        for (int i = 2; i < 11; ++i) {
            countArr[i] += countArr[i - 1];
        }

        for (int i = 0; i < input.size(); ++i) {
            int rdn = rad(input[i], digit);
            prom[countArr[rdn != -1 ? rdn + 1 : 0]] = input[i];
            countArr[rdn != -1 ? rdn + 1 : (0)] ++;
        }

        input = prom;
        prom = std::vector<int>(input.size());

        digit++;
    }

    return(input);
}

std::string prefix(std::string line, int s) {
    //������� � ������, �� s (�� �������)

    std::string ret;
    for (int i = 0; i < s; ++i) {
        ret.push_back(line[i]);
    }
    return ret;
}



TsuffixTrie* constructTree(std::string text) {
    //������ ���������� ������ �� text

    TsuffixTrie* trie = new TsuffixTrie(text);

    for (int i = 0; i < text.size(); ++i) {
        int k = trie->Add(text, i);
        //trie->Print();
        //printf("----------------\n");   
        if (k != 0) {
            return nullptr;
        }
    }

    return trie;
}

int main() {

    std::string exmp;
    std::string text;
    std::cin >> exmp;
    std::cin >> text;
    text.push_back('$');

    TsuffixTrie* trie = constructTree(text);

    if (trie == nullptr) {
        printf("allocation error!\n");
        return -1;
    }

    std::vector<int> res = trie->locate(exmp);
    res = radixSort(res);

    for (int i = 0; i < res.size(); ++i) {
        printf("%d\n", res[i] + 1);
    }
}