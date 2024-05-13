#include "myedge.h"

MyEdge::MyEdge(MyNode *s, MyNode *e, QGraphicsItem *parent): QGraphicsLineItem(parent), startNode(s), endNode(e) {
    setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), 3, Qt::SolidLine));
}

void MyEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    setLine(QLineF(startNode->sceneBoundingRect().center(), endNode->sceneBoundingRect().center()));
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsLineItem::paint(painter, option, widget);
}
