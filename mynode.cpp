#include "mynode.h"

#include "myscene.h"
#include "myedge.h"

MyNode::MyNode(MyScene *_scene, QString _name, QGraphicsItem *parent): QGraphicsEllipseItem(0, 0, 50, 50, parent), scene(_scene) {
    // Make sure that 2 * radius == what in the constructor above ||.
    setBrush(Qt::white);

    if (!_name.isNull()) {
        name = new QGraphicsSimpleTextItem(_name, this);
        name->setFont(QFont("Microsoft Yahei", -1, QFont::Bold));
    }
    updateMode();
    setAcceptHoverEvents(1);
}

void MyNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    name->setPos(boundingRect().center() - name->boundingRect().center());
    QStyleOptionGraphicsItem *op = (QStyleOptionGraphicsItem *)option;
    QColor Color = color;
    if (option->state & (QStyle::State_Selected | QStyle::State_MouseOver)) {
        Color = QColor(57, 197, 187);
    }
    setPen(QPen(QBrush(Color, Qt::SolidPattern), penSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    op->state &= ~QStyle::State_Selected;

    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsEllipseItem::paint(painter, op, widget);
}

void MyNode::updateMode() {
    if (scene->curMode == MyScene::MoveMode) {
        setFlag(QGraphicsItem::ItemIsMovable, 1);
        // setFlag(QGraphicsItem::ItemIsSelectable, 0);
    }
    if (scene->curMode == MyScene::SelectMode) {
        setFlag(QGraphicsItem::ItemIsMovable, 1);
        // setFlag(QGraphicsItem::ItemIsSelectable, 1);
    }
    if (scene->curMode == MyScene::AddMode) {
        setFlag(QGraphicsItem::ItemIsMovable, 0);
        // setFlag(QGraphicsItem::ItemIsSelectable, 0);
    }
    if (scene->curMode == MyScene::DeleteMode) {
        setFlag(QGraphicsItem::ItemIsMovable, 0);
        // setFlag(QGraphicsItem::ItemIsSelectable, 1);
    }
    setFlag(QGraphicsItem::ItemIsSelectable, 1);
}
