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


    MyNode* u = addNode(0, 0);

    MyNode* v = addNode(80, 80);

    addEdge(u, v);

    MyNode *w = addNode(-120, 40);
    addEdge(u, w);

    switchMode(curMode);
}

MyNode* MyScene::addNode(qreal x, qreal y) {
    MyNode* node = new MyNode(this);
    addItem(node);
    nodes.insert(node);
    node->setPos(x, y);
    return node;
}
MyEdge* MyScene::addEdge(MyNode *u, MyNode *v) {
    MyEdge* edge = new MyEdge(this, u, v);
    addItem(edge);
    edges.insert(edge);
    u->outEdge.insert(edge), v->inEdge.insert(edge);
    return edge;
}
void MyScene::delEdge(MyEdge *e) {
    removeItem(e);
    edges.erase(e);
    e->startNode->outEdge.erase(e);
    e->endNode->inEdge.erase(e);
    delete e;
}
void MyScene::delNode(MyNode *n) {
    removeItem(n);
    nodes.erase(n);
    std::vector <MyEdge*> es;
    for (auto e: n->outEdge) es.push_back(e);
    for (auto e: n->inEdge) es.push_back(e);
    n->inEdge.clear(), n->outEdge.clear();
    for (auto e: es) delEdge(e);
    delete n;
}
void MyScene::delItem(QGraphicsItem *x) {
    if (x->type() == MyNode::Type) {
        MyNode *n = qgraphicsitem_cast<MyNode*>(x);
        qDebug() << n << Qt::endl;
        delNode(n);
    }
    if (x->type() == MyEdge::Type) {
        MyEdge *e = qgraphicsitem_cast<MyEdge*>(x);
        qDebug() << e << Qt::endl;
        delEdge(e);
    }
}

void MyScene::switchMode(CursorMode mode) {
    // exit current mode
    if (curMode == MoveMode) {
        for (auto n: nodes) n->setFlag(QGraphicsItem::ItemIsMovable, 0);
        qgView->setDragMode(QGraphicsView::NoDrag);
    }
    if (curMode == DeleteMode) {
        for (auto n: nodes) n->setFlag(QGraphicsItem::ItemIsSelectable, 0);
        for (auto e: edges) e->setFlag(QGraphicsItem::ItemIsSelectable, 0);
    }

    // enter new mode
    curMode = mode;
    if (curMode == MoveMode) {
        for (auto n: nodes) n->setFlag(QGraphicsItem::ItemIsMovable);
        qgView->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    if (curMode == DeleteMode) {
        for (auto n: nodes) n->setFlag(QGraphicsItem::ItemIsSelectable);
        for (auto e: edges) e->setFlag(QGraphicsItem::ItemIsSelectable);
        qgView->setDragMode(QGraphicsView::RubberBandDrag);
    }
}

void MyScene::toggleDirect() {
    directed ^= 1;
    for (auto e: edges) e->update();
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    const int expandSize = 100;
    setSceneRect(sceneRect().united(itemsBoundingRect().adjusted(-expandSize, -expandSize, expandSize, expandSize)));

    QPointF clicked_point = event->scenePos();
    qDebug() << clicked_point << ' ';
    QGraphicsItem * want = nullptr;
    QList <QGraphicsItem*> clicked_items = items(clicked_point);
    for (auto it: clicked_items)
        if (it->type() == MyNode::Type || it->type() == MyEdge::Type) {
            want = it;
            break;
        }

    qDebug() << want << Qt::endl;
    if (curMode == MoveMode) {
        if (want) {
            qDebug() << want->scenePos() << Qt::endl;
            want->setZValue(standard_z);
            standard_z += 1e-4;
            qDebug() << standard_z << Qt::endl;
        }
        else qDebug() << "nullptr" << Qt::endl;
    }
    if (curMode == DeleteMode) {
        if (want) delItem(want);
    }

    dragged = 1;
    lastClickedPoint = event->scenePos();
    // qDebug() << "dragged " << lastClickedPoint << Qt::endl;
    increseBoundray();

    QGraphicsScene::mousePressEvent(event);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    // qDebug() << "released " << lastClickedPoint << Qt::endl;
    if(dragged) dragged = 0;
    if (curMode == DeleteMode) {
        QList <QGraphicsItem*> selected = selectedItems();
        std::sort(selected.begin(), selected.end(), [&](QGraphicsItem* u, QGraphicsItem *v) { return u->type() > v->type(); });
        // qDebug() << selected << Qt::endl;
        for (auto x: selected) delItem(x);

    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(!dragged){
        return;
    }
    // qDebug() << "dragging " << Qt::endl;
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
