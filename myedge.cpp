#include "myedge.h"

#include "myscene.h"
#include "mynode.h"

MyEdge::MyEdge(MyScene *_scene, MyNode *s, MyNode *e, QString _weight, QGraphicsItem *parent):
    QGraphicsLineItem(parent), startNode(s), endNode(e), scene(_scene) {
    setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), penSize, Qt::SolidLine));
    weight = new QGraphicsSimpleTextItem(_weight, this);
    updateMode();
}

void MyEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QLineF l(startNode->sceneBoundingRect().center(), endNode->sceneBoundingRect().center());

    QPointF p1 = l.p1() + QLineF::fromPolar(startNode->radius + startNode->penSize - 4, l.angle()).p2();
    QPointF p2 = l.p2() - QLineF::fromPolar(endNode->radius + endNode->penSize - 4, l.angle()).p2();
    setLine(QLineF(p1, p2));

    weight->setPos(boundingRect().center() - weight->boundingRect().center());
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsLineItem::paint(painter, option, widget);

    if (scene->directed) {
        QPainterPath path;
        path.moveTo(p2);
        QLineF leftl = l.fromPolar(15, l.angle() + 160);
        QLineF rightl = l.fromPolar(15, l.angle() - 160);
        path.lineTo(p2 + leftl.p2()); path.lineTo(p2 + rightl.p2()); path.lineTo(p2);

        painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
        painter->drawPath(path);
    }
}

void MyEdge::updateMode() {
    if (scene->curMode == MyScene::MoveMode) {
        setFlag(QGraphicsItem::ItemIsMovable, 0);
        setFlag(QGraphicsItem::ItemIsSelectable, 0);
    }
    if (scene->curMode == MyScene::SelectMode) {
        setFlag(QGraphicsItem::ItemIsMovable, 0);
        setFlag(QGraphicsItem::ItemIsSelectable, 1);
    }
    if (scene->curMode == MyScene::AddMode) {
        setFlag(QGraphicsItem::ItemIsMovable, 0);
        setFlag(QGraphicsItem::ItemIsSelectable, 0);
    }
    if (scene->curMode == MyScene::DeleteMode) {
        setFlag(QGraphicsItem::ItemIsMovable, 0);
        setFlag(QGraphicsItem::ItemIsSelectable, 1);
    }
}
