#ifndef MYEDGE_H
#define MYEDGE_H

#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>

class MyNode;

class MyEdge : public QGraphicsLineItem
{
public:
    MyNode *startNode, *endNode;
    QGraphicsSimpleTextItem *weight;

    MyEdge(MyNode *s, MyNode *e, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    MyNode * operator() (MyNode *u) { return u == startNode ? endNode : startNode; }
};

#endif // MYEDGE_H
