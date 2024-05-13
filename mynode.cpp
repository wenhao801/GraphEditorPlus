#include "mynode.h"

void MyNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

MyNode::MyNode(QGraphicsItem *parent): QGraphicsEllipseItem(0, 0, 50, 50, parent) {
    setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setBrush(Qt::white);
    setFlag(QGraphicsItem::ItemIsMovable);

    name = new QGraphicsSimpleTextItem("NodeName", this);
    name->setFont(QFont("Microsoft Yahei", -1, QFont::Bold));
    name->setPos(boundingRect().center() - name->boundingRect().center());
}