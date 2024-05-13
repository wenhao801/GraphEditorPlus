#include "myscene.h"
#include <QGraphicsSceneMouseEvent>

MyScene::MyScene(QObject *parent)
    : QGraphicsScene{parent}
{
    setSceneRect(-500, -500, 1000, 1000);
}

MyNode* MyScene::addNode(qreal x, qreal y) {
    MyNode* node = new MyNode();
    addItem(node);
    node->setPos(x, y);
    return node;
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);

    const int expandSize = 100;
    setSceneRect(sceneRect().united(itemsBoundingRect().adjusted(-expandSize, -expandSize, expandSize, expandSize)));

    QLineF topLine(sceneRect().topLeft(),
                   sceneRect().topRight());
    QLineF leftLine(sceneRect().topLeft(),
                    sceneRect().bottomLeft());
    QLineF rightLine(sceneRect().topRight(),
                     sceneRect().bottomRight());
    QLineF bottomLine(sceneRect().bottomLeft(),
                      sceneRect().bottomRight());

    // QPen myPen = QPen(Qt::red);

    // addLine(topLine, myPen);
    // addLine(leftLine, myPen);
    // addLine(rightLine, myPen);
    // addLine(bottomLine, myPen);

    qDebug() << width() << ' ' << height() << Qt::endl;
    qDebug() << sceneRect().bottom() << ' ' << sceneRect().top() << ' ' << sceneRect().left() << ' ' << sceneRect().right() << Qt::endl;
    qDebug() << itemsBoundingRect().bottom() << ' ' << itemsBoundingRect().top() << ' ' << itemsBoundingRect().left() << ' ' << itemsBoundingRect().right() << Qt::endl;
}
