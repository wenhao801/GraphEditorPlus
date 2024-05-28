#include "mynode.h"

#include "myscene.h"
#include "myedge.h"

MyNode::MyNode(MyScene *_scene, QString _name, QGraphicsItem *parent): QGraphicsEllipseItem(0, 0, 50, 50, parent), scene(_scene) {
    // Make sure that 2 * radius == what in the constructor above ||.
    setPen(QPen(QBrush(Qt::black, Qt::SolidPattern), penSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setBrush(Qt::white);

    name = new QGraphicsSimpleTextItem(_name, this);
    name->setFont(QFont("Microsoft Yahei", -1, QFont::Bold));
    name->setPos(boundingRect().center() - name->boundingRect().center());
    updateMode();
}

void MyNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

void MyNode::updateMode() {
    if (scene->curMode == MyScene::MoveMode) {
        setFlag(QGraphicsItem::ItemIsMovable, 1);
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
