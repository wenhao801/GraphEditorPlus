#ifndef MYEDGE_H
#define MYEDGE_H

#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>


class MyScene;
class MyNode;

class MyEdge : public QGraphicsLineItem
{
    const int penSize = 3;
public:
    MyNode *startNode, *endNode;
    QGraphicsSimpleTextItem *weight;

    MyScene *scene;

    MyEdge(MyScene *scene, MyNode *s, MyNode *e, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    MyNode * operator() (MyNode *u) { return u == startNode ? endNode : startNode; }
};

#endif // MYEDGE_H
