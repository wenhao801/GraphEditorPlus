#include "mynode.h"

#include "myscene.h"
#include "myedge.h"

MyNode::MyNode(MyScene *_scene, QGraphicsItem *parent): QGraphicsEllipseItem(0, 0, 50, 50, parent), scene(_scene) {
    // Make sure that 2 * radius == what in the constructor above ||.
    setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), penSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setBrush(Qt::white);

    name = new QGraphicsSimpleTextItem("NodeName", this);
    name->setFont(QFont("Microsoft Yahei", -1, QFont::Bold));
    name->setPos(boundingRect().center() - name->boundingRect().center());
}

void MyNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsEllipseItem::paint(painter, option, widget);
}
