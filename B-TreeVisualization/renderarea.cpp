#include "renderarea.h"


RenderArea::RenderArea(QWidget *parent, BTree* btree)
    : QWidget{parent}, btree(btree)
{
    fSize = 13;
    squareSize = 35;
    rowSpace = 70;

    nodeColor = Qt::white;
    nodeSearchingColor = Qt::red;
    textColor = Qt::black;
    foundTextColor = Qt::green;

    brush = new QBrush();
    pen = new QPen();
    pen->setWidth(3);
}

RenderArea::~RenderArea()
{
    delete pen;
    delete brush;
    //delete btree;
}

void RenderArea::paintEvent (QPaintEvent*)
{
    if (btree->GetRoot() == NULL) return;
    QPainter* painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);
    brush->setColor(this->nodeColor);
    brush->setStyle(Qt::SolidPattern);
    pen->setColor(this->textColor);
    painter->setBrush(*brush);
    painter->setPen(*pen);
    DrawBTree(painter, btree->GetRoot(), X, Y);
    painter->end();
}

void RenderArea::SetMaxDeg(int maxDeg)
{
    if (btree->GetRoot() != NULL) btree->Clear();
    btree->SetOrder(maxDeg);
}

void RenderArea::constructNode(QPainter* painter, QString s, double x, double y)
{

    QRectF rectNode = QRectF(QPointF(x,y),QSizeF(squareSize, squareSize));
    painter->drawRect(rectNode);
    while (s.size() < 4) s = "0" + s;
    QStaticText text;
    text.setTextWidth(squareSize - 2.0);
    text.setText(s);
    painter->drawStaticText(QPoint(x + 4.0, y + 7.0), text);
}

void RenderArea::DrawBTree(QPainter* painter, BNode* root, double x, double y)
{
    if (!root) return;
    brush->setColor(nodeColor);
    brush->setColor(textColor);
    for (int i = 0;i < root->GetSize(); ++i)
    {
        constructNode(painter, toString(root->GetKey(i)), x + i * squareSize, y );
    }

    double startY = y + 2 * fSize;
    if (!root->isLeaf())
    {
        for (int i=0;i<root->GetSize() + 1;++i)
        {
            double startLine = x + i * squareSize;
            double startChildNode, endLine;

            if (compareRootSize(i, root) == 1)
            {
                startChildNode = startLine + (btree->GetOrder()) * (btree->GetHeight(root->GetChild(i))-1) * squareSize / 2;
                endLine = startChildNode + ((double) root->GetChild(i)->GetSize()) / 2 * squareSize;
            }
            else if (compareRootSize(i,root)==0)
            {
                endLine = startLine - (btree->GetOrder()) * (btree->GetHeight(root->GetChild(i))-1) * squareSize / 2
                        - ((double) root->GetChild(i)->GetSize()) / 2 * squareSize;
                startChildNode = endLine - ((double) root->GetChild(i)->GetSize()) / 2 * squareSize;
            }
            else
            {
                startChildNode = startLine - ((double) root->GetChild(i)->GetSize()) / 2 * squareSize;
                endLine = startLine;
            }
            if (i == 0)
            {
                startChildNode = startChildNode - squareSize * 2;
                endLine = endLine - squareSize * 2;
            } else if (i == root->GetSize()) {
                startChildNode = startChildNode + squareSize * 2;
                endLine = endLine + squareSize * 2;
            }
            if (root->GetChild(i) != NULL)
            {
                QLineF line = QLineF(startLine, startY, endLine, y + rowSpace);
                painter->drawLine(line);
            }
            DrawBTree(painter, root->GetChild(i), startChildNode, y + rowSpace);
        }
    }
}

QString RenderArea::toString(int x)
{
    QString res = "";
    while (x)
    {
        res = char(x % 10 + '0') + res;
        x /= 10;
    }
    while (sz(res) < 4) res = "0" + res;
    return res;
}

int RenderArea::compareRootSize(int i, BNode* root)
{
    if((double) i > ((double) root->GetSize()) / 2) return 1;
    if((double) i < ((double) root->GetSize()) / 2) return 0;
    return -1;
}

void RenderArea::Insert(int key)
{
    btree->Insert(key);
}

bool RenderArea::Remove(int key)
{
    if (!btree->Remove(key)) return false;
    return true;
}

bool RenderArea::Search(int key)
{
    if (btree->Contains(key) == NULL)
        return false;
    return true;
}

void RenderArea::Clear()
{
    btree->Clear();
}

QString RenderArea::Print()
{
    if (!btree->GetRoot()) return "";
    return btree->Traverse();
}
