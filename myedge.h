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
    enum { Type = UserType + 2 };
    int type() const override { return Type; }

    MyNode *startNode, *endNode;
    QGraphicsSimpleTextItem *weight;

    MyScene *scene;

    MyEdge(MyScene *, MyNode *s, MyNode *e, QString _weight, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void updateMode();
    MyNode * operator() (MyNode *u) { return u == startNode ? endNode : startNode; }
};

#endif // MYEDGE_H
