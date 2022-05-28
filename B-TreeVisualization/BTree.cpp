#include "BTree.h"
#include <QDebug>

QString BTree::Traverse()
{
    if (root != NULL) return root->Traverse();
    return "";
}

BNode* BTree::Contains(int key)
{
    if (!root) return NULL;
    return root->Contains(key);
}
void BTree::Insert(int key)
{
    if (!root)
    {
        root = new BNode(minDeg, 1);
        root->keys.pb(key);
        return;
    }
    root->Insert(key);
    if (sz(root->keys) == 2 * minDeg - 1)
    {
        BNode* nroot = new BNode(minDeg, 0);
        nroot->subset.pb(root);
        nroot->fixExcess(0);
        root = nroot;
    }
}

bool BTree::Remove(int key)
{
    if (!root)
    {
        //cout << "B-Tree is empty!\n";
        return false;
    }
    bool removed = root->Remove(key);
    if (!removed) return false;
    if (!sz(root->keys))
    {
        BNode* tmp = root;
        root = (root->leaf ? NULL : root->subset[0]);
        delete tmp;
    }
    return true;
}

void BTree::Clear()
{
    if (root == NULL) return;
    root->Clear();
    //delete root;
    root = NULL;
}

BNode* BTree::GetRoot()
{
    return root;
}

int BTree::GetHeight(BNode *node)
{
    if (!root) return 0;
    int height = 0;
    BNode* cur = node;
    while (cur != NULL)
    {
        height++;
        if (cur->leaf) break;
        cur = cur->subset[0];
    }
    return height;
}
