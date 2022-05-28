#pragma once
#include "BNode.h"

class BTree
{
private:
    int minDeg;
    BNode* root;
public:
    BTree(int maxDeg = 2) : minDeg((maxDeg + 1) / 2) { root = NULL; }
    //~BTree() {delete root;};
    QString Traverse();
    BNode* Contains(int key);
    BNode* GetRoot();
    void SetOrder(int order){this->minDeg = (order + 1) / 2;}
    int GetOrder() {return minDeg * 2 - 1;};
    int GetHeight(BNode* node);
    void Insert(int key);
    bool Remove(int key);
    void Clear();
};

