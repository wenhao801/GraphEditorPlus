#include "myscene.h"
#include <QGraphicsSceneMouseEvent>
#include<QListWidget>

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

    const int expandSize = 100;
    setSceneRect(sceneRect().united(itemsBoundingRect().adjusted(-expandSize, -expandSize, expandSize, expandSize)));

    QPointF clicked_point = event->buttonDownScenePos(Qt::LeftButton);
    qDebug() << clicked_point << ' ';
    QGraphicsItem * want = itemAt(clicked_point, QTransform());
    if (want) {
        qDebug() << want->scenePos() << Qt::endl;
        want->setZValue(standard_z);
        standard_z += 1e-5;
        qDebug() << standard_z << Qt::endl;
    }
    else qDebug() << "nullptr" << Qt::endl;


    QGraphicsScene::mousePressEvent(event);
}
