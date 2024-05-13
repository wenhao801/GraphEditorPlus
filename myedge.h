#ifndef MYEDGE_H
#define MYEDGE_H

#include <QGraphicsLineItem>
#include "mynode.h"

class MyEdge : public QGraphicsLineItem
{
public:
    MyNode *startNode, *endNode;

    MyEdge(MyNode *s, MyNode *e, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // MYEDGE_H
