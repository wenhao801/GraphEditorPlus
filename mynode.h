#ifndef MYNODE_H
#define MYNODE_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>

class MyNode : public QGraphicsEllipseItem
{
public:
    MyNode(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QGraphicsSimpleTextItem *name;
};

#endif // MYNODE_H
