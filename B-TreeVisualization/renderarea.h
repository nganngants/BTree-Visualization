#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include "BTree.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QStaticText>
#include <QLineF>
#include <QThread>
#include <QPropertyAnimation>
#include <QApplication>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr, BTree* btree = new BTree());
    ~RenderArea();
    void DrawBTree(QPainter* painter, BNode *root, double x, double y);
    bool compareLength(QString s, int x);
    int compareRootSize(int i, BNode* root);

    void SetX(double x) {X = x;}
    void SetY(double y) {Y = y;}
    void SetMaxDeg(int maxDeg);

    void Insert(int key);
    bool Remove(int key);
    bool Search(int key);
    void Clear();
    QString Print();

    static QString toString(int x);

signals:

protected:
    void paintEvent(QPaintEvent* event) override;
private:
    BTree *btree;
    double X, Y; //coordinate
    int fSize; //font size
    int squareSize;
    int rowSpace;
    QColor nodeColor;
    QColor nodeSearchingColor;
    QColor textColor;
    QColor foundTextColor;

    QPen* pen;
    QBrush* brush;
    void constructNode(QPainter* painter, QString s, double x, double y);

};

#endif // RENDERAREA_H
