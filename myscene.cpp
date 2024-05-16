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
MyEdge* MyScene::addEdge(MyNode *u, MyNode *v) {
    MyEdge* edge = new MyEdge(u, v);
    addItem(edge);
    u->outEdge.insert(edge), v->inEdge.insert(edge);
    return edge;
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);

    const int expandSize = 100;
    setSceneRect(sceneRect().united(itemsBoundingRect().adjusted(-expandSize, -expandSize, expandSize, expandSize)));

    // adjusted here


    qDebug() << width() << ' ' << height() << Qt::endl;
    qDebug() << sceneRect().bottom() << ' ' << sceneRect().top() << ' ' << sceneRect().left() << ' ' << sceneRect().right() << Qt::endl;
    qDebug() << itemsBoundingRect().bottom() << ' ' << itemsBoundingRect().top() << ' ' << itemsBoundingRect().left() << ' ' << itemsBoundingRect().right() << Qt::endl;
}
