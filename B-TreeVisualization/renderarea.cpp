#include "renderarea.h"


RenderArea::RenderArea(QWidget *parent, BTree* btree)
    : QWidget{parent}, btree(btree)
{
    fSize = 13;
    squareSize = 35;
    rowSpace = 70;

    nodeColor = Qt::white;
    textColor = Qt::black;

    brush = new QBrush();
    pen = new QPen();
    pen->setWidth(3);
    brush->setColor(this->nodeColor);
    brush->setStyle(Qt::SolidPattern);
    pen->setColor(this->textColor);
}

RenderArea::~RenderArea()
{
    delete pen;
    delete brush;
}

void RenderArea::paintEvent (QPaintEvent*)
{
    if (btree->GetRoot() == NULL) return;
    QPainter* painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);
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

int RenderArea::DrawBTree(QPainter* painter, BNode* root, double x, double y)
{
    if (root == NULL) return 0;
    if (!root->isLeaf())
    {
        vector<QPoint> children;
        double oldStartX = x, nextStartX;

        //call recursion to draw children nodes first
        for (int i=0;i<=root->GetSize();++i)
        {
            nextStartX = DrawBTree(painter,root->GetChild(i),oldStartX,y + rowSpace);
            int endLine = (nextStartX + oldStartX) / 2.0;
            children.push_back(QPoint(endLine,y + rowSpace)); //keeps children nodes position
            oldStartX = nextStartX + squareSize;
        }

        //draw this node
        double curStartX = (oldStartX + x) / 2.0 - (1.0 * root->GetSize()) * 1.0 * squareSize;
        for (int i=0;i<root->GetSize();++i)
            constructNode(painter, toString(root->GetKey(i)), curStartX + i * squareSize, y );

        //draw lines
        double startY = y + 2 * fSize;
        double startLine;

        for (int i=0;i<=root->GetSize();++i)
        {
            startLine = curStartX + i * squareSize;
            QLineF line = QLineF(QPoint(startLine,startY), children[i]);
            painter->drawLine(line);
        }
        return nextStartX;
    }
    else
    {
        for (int i=0;i<root->GetSize();++i)
            constructNode(painter, toString(root->GetKey(i)), x + i * squareSize, y );
        return (x + root->GetSize() * squareSize);
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
