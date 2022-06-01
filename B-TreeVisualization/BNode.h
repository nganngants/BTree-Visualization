
#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <QString>

using namespace std;

#define pb push_back
#define sz(a) (int)a.size()
#define fu(i,a,b) for(int i=a;i<=b;++i)
#define fd(i,a,b) for(int i=a;i>=b;--i)

class BNode
{
private:
    vector <int> keys;
    vector <BNode*> subset;
    int minDeg;
    bool leaf;
    int FindGE(int key);
    void fixExcess(int i);
    void fixShortage(int i);
    int RemoveBiggest();
public:
    BNode(int minDeg, bool leaf) : minDeg(minDeg), leaf(leaf) {};
    QString Traverse();
    BNode* Contains(int key);

    void Insert(int key);
    bool Remove(int key);
    void Clear();
    int GetKeysSize() {return sz(keys);};
    BNode* GetChild(int i) {if (i < sz(subset)) return subset[i]; return NULL;}
    int GetKey(int i) {if (i < sz(keys)) return keys[i]; return 0;}
    int GetOrder() {return minDeg * 2 - 1;}
    bool isLeaf() {return leaf;}
    friend class BTree;
};

