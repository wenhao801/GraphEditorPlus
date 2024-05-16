#include "myscene.h"
#include <QGraphicsSceneMouseEvent>
#include<QListWidget>

MyScene::MyScene(QObject *parent, QGraphicsView *q)
    : QGraphicsScene(parent), qgView(q)
{
    setSceneRect(-500, -500, 1000, 1000);

    dragged = 0;
    boundrayWidth = 10;
    extendAmount = 1000;
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


    // addLine(topLine, myPen);
    // addLine(leftLine, myPen);
    // addLine(rightLine, myPen);
    // addLine(bottomLine, myPen);

    qDebug() << width() << ' ' << height() << Qt::endl;
    qDebug() << sceneRect().bottom() << ' ' << sceneRect().top() << ' ' << sceneRect().left() << ' ' << sceneRect().right() << Qt::endl;
    qDebug() << itemsBoundingRect().bottom() << ' ' << itemsBoundingRect().top() << ' ' << itemsBoundingRect().left() << ' ' << itemsBoundingRect().right() << Qt::endl;

    dragged = 1;
    lastClickedPoint = event->scenePos();
    qDebug() << "dragged " << lastClickedPoint << Qt::endl;
    increseBoundray();

    QGraphicsScene::mousePressEvent(event);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mouseReleaseEvent(event);
    qDebug() << "released " << lastClickedPoint << Qt::endl;
    if(dragged) dragged = 0;
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(!dragged){
        return;
    }
    qDebug() << "dragging " << Qt::endl;
    increseBoundray();
    QGraphicsScene::mouseMoveEvent(event);
}

void MyScene::increseBoundray(){
    QRectF currentSceneRect = sceneRect();
    bool needUpdate = false;

    if (qgView->mapToScene(qgView->viewport()->rect().bottomRight()).x() > currentSceneRect.right() - boundrayWidth) {
        currentSceneRect.setRight(currentSceneRect.right() + extendAmount);
        needUpdate = true;
    }
    if (qgView->mapToScene(qgView->viewport()->rect().bottomRight()).y() > currentSceneRect.bottom() - boundrayWidth) {
        currentSceneRect.setBottom(currentSceneRect.bottom() + extendAmount);
        needUpdate = true;
    }
    if (qgView->mapToScene(qgView->viewport()->rect().topLeft()).x() < currentSceneRect.left() + boundrayWidth) {
        currentSceneRect.setLeft(currentSceneRect.left() - extendAmount);
        needUpdate = true;
    }
    if (qgView->mapToScene(qgView->viewport()->rect().topLeft()).y() < currentSceneRect.top() + boundrayWidth) {
        currentSceneRect.setTop(currentSceneRect.top() - extendAmount);
        needUpdate = true;
    }

    if (needUpdate) {
        setSceneRect(currentSceneRect);
    }
}
