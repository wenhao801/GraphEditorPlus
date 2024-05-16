#include "myedge.h"

#include "mynode.h"

MyEdge::MyEdge(MyNode *s, MyNode *e, QGraphicsItem *parent): QGraphicsLineItem(parent), startNode(s), endNode(e) {
    setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), penSize, Qt::SolidLine));
    weight = new QGraphicsSimpleTextItem("EdgeWeight", this);
}

void MyEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QLineF l(startNode->sceneBoundingRect().center(), endNode->sceneBoundingRect().center());

    QPointF p1 = l.p1() + QLineF::fromPolar(startNode->radius + startNode->penSize - 4, l.angle()).p2();
    QPointF p2 = l.p2() - QLineF::fromPolar(endNode->radius + endNode->penSize - 4, l.angle()).p2();
    setLine(QLineF(p1, p2));

    weight->setPos(boundingRect().center() - weight->boundingRect().center());
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsLineItem::paint(painter, option, widget);

    QPainterPath path;
    path.moveTo(p2);
    QLineF leftl = l.fromPolar(15, l.angle() + 160);
    QLineF rightl = l.fromPolar(15, l.angle() - 160);
    path.lineTo(p2 + leftl.p2()); path.lineTo(p2 + rightl.p2()); path.lineTo(p2);

    painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter->drawPath(path);
}
