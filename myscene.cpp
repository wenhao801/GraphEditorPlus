#include "myscene.h"
#include <QGraphicsSceneMouseEvent>
#include<QListWidget>

#include <QRandomGenerator>

MyScene::MyScene(QObject *parent, QGraphicsView *q)
    : QGraphicsScene(parent), qgView(q)
{
    setSceneRect(-500, -500, 1000, 1000);

    dragged = 0;

    std::vector <MyNode*> tmp;
    for (int i = 0; i < 5; i++) {
        MyNode *now = addNode(QRandomGenerator::global()->bounded(-200, 200), QRandomGenerator::global()->bounded(-200, 200));
        if (i) addEdge(now, tmp[QRandomGenerator::global()->bounded(0, i)]);
        tmp.push_back(now);
    }

    // MyNode* u = addNode(0, 0);

    // MyNode* v = addNode(80, 80);

    // addEdge(u, v);

    // MyNode *w = addNode(-120, 40);
    // addEdge(u, w);

    switchMode(curMode);
}

MyNode* MyScene::addNode(qreal x, qreal y, QString name) {
    if (name.isNull()) {
        while (ids.count(QString::number(++defaultNodeID)));
        name = QString::number(defaultNodeID);
    }
    if (ids.count(name)) return nullptr;
    MyNode* node = new MyNode(this, name);
    ids[name] = node;
    addItem(node);
    nodes.insert(node);
    node->setPos(x, y);
    return node;
}
MyEdge* MyScene::addEdge(MyNode *u, MyNode *v, QString weight) {
    MyEdge* edge = new MyEdge(this, u, v, weight);
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
    ids.erase(ids.find(n->name->text()));
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

void MyScene::insertFromText(QString text) {
    QStringList lines = text.split('\n', Qt::SkipEmptyParts);
    for (auto line: lines) {
        QStringList words = line.split(' ', Qt::SkipEmptyParts);
        if (words.size() <= 3) {
            addNode(QRandomGenerator::global()->bounded(-200, 200), QRandomGenerator::global()->bounded(-200, 200), words[0]);
            if (words.size() >= 2) {
                addNode(QRandomGenerator::global()->bounded(-200, 200), QRandomGenerator::global()->bounded(-200, 200), words[1]);
                addEdge(ids[words[0]], ids[words[1]], words.size() == 3 ? words[2] : nullptr);
            }
        }
    }
}

void MyScene::switchMode(CursorMode mode) {
    // enter new mode
    curMode = mode;
    if (curMode == MoveMode) {
        qgView->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    if (curMode == SelectMode) {
        qgView->setDragMode(QGraphicsView::RubberBandDrag);
    }
    if (curMode == AddMode) {
        qgView->setDragMode(QGraphicsView::NoDrag);
    }
    if (curMode == DeleteMode) {
        qgView->setDragMode(QGraphicsView::RubberBandDrag);
    }
    for (auto n: nodes) n->updateMode();
    for (auto e: edges) e->updateMode();
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
    if (curMode == AddMode) {
        if (want && want->type() == MyNode::Type) {
            DragNode =  qgraphicsitem_cast<MyNode*>(want);
            ADDedge = true;
            HiddenNode = new MyNode(this, nullptr);
            HiddenNode->setPos(clicked_point);
            myline = new MyEdge(this,DragNode,HiddenNode,nullptr);
            addItem(HiddenNode), addItem(myline);
            HiddenNode->setVisible(false);
        }
        else {
            addNode(clicked_point.x(), clicked_point.y());
        }
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
    if (curMode == AddMode && ADDedge) {
        QPointF released_point = event->scenePos();
        QGraphicsItem * want = nullptr;
        QList <QGraphicsItem*>  released_items = items(released_point);
        for (auto it: released_items)
            if (it->type() == MyNode::Type || it->type() == MyEdge::Type) {
                want = it;
                break;
            }
        EndNode = qgraphicsitem_cast<MyNode*>(want);
        if (want && EndNode != DragNode) {
            addEdge(DragNode, EndNode);
            ADDedge = false;
        }
        // delEdge(myline);
        // delNode(HiddenNode);
        delete HiddenNode;
        delete myline;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(dragged){
        increseBoundray();
    }
    // qDebug() << "dragging " << Qt::endl;
    if (curMode == AddMode && ADDedge) {
        QPointF CurPos = event->scenePos();
        HiddenNode->setPos(CurPos);
        myline->update();
    }
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
