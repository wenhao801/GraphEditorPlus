#include "mynode.h"

MyNode::MyNode(qreal x, qreal y, QGraphicsItem *parent): QGraphicsEllipseItem(x, y, 50, 50, parent) {
    setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setBrush(Qt::white);
    setFlag(QGraphicsItem::ItemIsMovable);
}
