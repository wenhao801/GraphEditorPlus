#ifndef MYNODE_H
#define MYNODE_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QFont>

class MyNode : public QGraphicsEllipseItem
{
public:
    MyNode(qreal x, qreal y, QGraphicsItem *parent = nullptr);

    QGraphicsSimpleTextItem *name;
};

#endif // MYNODE_H
