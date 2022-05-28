#include "BNode.h"
#include "renderarea.h"
#include <QDebug>
int BNode::FindGE(int key)
{
    if (!sz(keys)) return 0;
    auto it = lower_bound(keys.begin(), keys.end(), key);
    if (it == keys.end()) return sz(keys);
    return (int)(it - keys.begin());
}

QString BNode::Traverse()
{
    QString ret = "";
    for (int i = 0; i < sz(keys); ++i)
    {
        if (!leaf)
        {
            ret += subset[i]->Traverse();
            if (ret.back() != ' ') ret += " ";
        }
        ret += RenderArea::toString(keys[i]) + " ";
    }
    if (!leaf)
    {
        ret += subset[sz(keys)]->Traverse();
        if (ret.back() != ' ') ret += " ";
    }

    return ret;
}

BNode* BNode::Contains(int key)
{
    int pos = FindGE(key);
    if (pos < sz(keys) && keys[pos] == key) return this;
    if (leaf) return NULL;
    return subset[pos]->Contains(key);
}

void BNode::fixExcess(int i)
{
    BNode* rnode = new BNode(minDeg, subset[i]->leaf);
    for (int j = 0; j < minDeg - 1; ++j)
        rnode->keys.pb(subset[i]->keys[j + minDeg]);
    if (!rnode->leaf)
    {
        for (int j = 0; j < minDeg; ++j)
            rnode->subset.pb(subset[i]->subset[j + minDeg]);
    }
    int mid = subset[i]->keys[minDeg - 1];
    subset[i]->keys.resize(minDeg - 1);
    subset[i]->subset.resize(minDeg);
    int pos = FindGE(mid);
    if (pos < sz(keys)) keys.insert(keys.begin() + pos, mid);
    else keys.pb(mid);
    if (i + 1 < sz(subset)) subset.insert(subset.begin() + i + 1, rnode);
    else subset.pb(rnode);
    //qDebug() << "after insert: " << subset[i + 1]->keys[0] << '\n';
}

void BNode::Insert(int key)
{
    int pos = FindGE(key);
    if (leaf) keys.insert(pos + keys.begin(), key);
    else subset[pos]->Insert(key);
    if (!leaf)
        if (sz(subset[pos]->keys) == 2 * minDeg - 1)
            fixExcess(pos);
}


bool BNode::Remove(int key)
{
    int pos = FindGE(key);
    bool found = (pos < sz(keys) && keys[pos] == key);
    if (this->leaf)
    {
        if (!found)
        {
            cout << "The key " << key << " does not exist!\n";
            return false;
        }
        this->keys.erase(this->keys.begin() + pos);
        return true;
    }
    else
    {
        if (!found)
        {
            bool removed = this->subset[pos]->Remove(key);
            if (!removed) return false;
            if (sz(subset[pos]->keys) < minDeg - 1)
                fixShortage(pos);
            return true;
        }
        keys[pos] = subset[pos]->RemoveBiggest();
        if (sz(subset[pos]->keys) < minDeg - 1)
            fixShortage(pos);
        return true;
    }
}

void BNode::fixShortage(int i)
{
    if (i > 0 && sz(subset[i - 1]->keys) >= minDeg)
    {
        subset[i]->keys.insert(subset[i]->keys.begin(), this->keys[i - 1]);
        keys[i - 1] = subset[i - 1]->keys.back();
        subset[i - 1]->keys.pop_back();
        if (!subset[i - 1]->leaf)
        {
            subset[i]->subset.insert(subset[i]->subset.begin(),
                subset[i - 1]->subset.back());
            subset[i - 1]->subset.pop_back();
        }
    }
    else if (i + 1 < sz(subset) && sz(subset[i + 1]->keys) >= minDeg)
    {
        subset[i]->keys.push_back(this->keys[i]);
        keys[i] = subset[i + 1]->keys[0];
        subset[i + 1]->keys.erase(subset[i + 1]->keys.begin());
        if (!subset[i + 1]->leaf)
        {
            subset[i]->subset.push_back(subset[i + 1]->subset.front());
            subset[i + 1]->subset.erase(subset[i + 1]->subset.begin());
        }
    }
    else if (i > 0 && sz(subset[i - 1]->keys) == minDeg - 1)
    {
        BNode* t = subset[i];
        BNode* t1 = subset[i - 1];
        t1->keys.pb(keys[i - 1]);
        keys.erase(keys.begin() + i - 1);
        t1->keys.insert(t1->keys.end(), t->keys.begin(), t->keys.end());
        t1->subset.insert(t1->subset.end(), t->subset.begin(), t->subset.end());
        subset.erase(subset.begin() + i);
        delete t;
    }
    else if (i + 1 < sz(subset) && sz(subset[i + 1]->keys) == minDeg - 1)
    {
        BNode* t = subset[i];
        BNode* t1 = subset[i + 1];
        t1->keys.insert(t1->keys.begin(), keys[i]);
        keys.erase(keys.begin() + i);
        t1->keys.insert(t1->keys.begin(), t->keys.begin(), t->keys.end());
        t1->subset.insert(t1->subset.begin(), t->subset.begin(), t->subset.end());
        subset.erase(subset.begin() + i);
        delete t;
    }
}

int BNode::RemoveBiggest()
{
    if (leaf)
    {
        int ret = keys.back();
        keys.pop_back();
        return ret;
    }
    int ret = subset.back()->RemoveBiggest();
    if (sz(subset.back()->keys) < minDeg)
        fixShortage(sz(subset) - 1);
    return ret;
}

void BNode::Clear()
{
    if (leaf) return;
    for (auto &it : subset)
        it -> Clear();
    for (auto &it : subset)
        delete it;
}
