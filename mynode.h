#ifndef MYNODE_H
#define MYNODE_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>

class MyNode : public QGraphicsEllipseItem
{
public:
    MyNode(qreal x, qreal y, QGraphicsItem *parent = nullptr);
};

#endif // MYNODE_H
