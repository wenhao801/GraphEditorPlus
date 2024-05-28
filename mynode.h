#ifndef MYNODE_H
#define MYNODE_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>

#include <set>

class MyScene;

class MyEdge;

class MyNode : public QGraphicsEllipseItem
{

public:
    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    MyNode(MyScene *, QString _name, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void updateMode();

    MyScene *scene;
    QGraphicsSimpleTextItem *name = nullptr;

    const int penSize = 5, radius = 25;

    std::set <MyEdge*> inEdge, outEdge;
};

#endif // MYNODE_H
