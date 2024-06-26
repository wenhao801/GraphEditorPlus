#include "myscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QListWidget>
#include "mainwindow.h"

#include <QRandomGenerator>

MyScene::MyScene(QObject *parent, QGraphicsView *q, QLabel *_node, QLabel *_edge, QLabel *sp)
    : QGraphicsScene(parent), qgView(q), nodeCount(_node), edgeCount(_edge), spStatus(sp), editWindow(this)
{
    setSceneRect(-500, -500, 1000, 1000);
    window = (MainWindow*) parent;

    dragged = 0;


    std::vector <MyNode*> tmp;
    for (int i = 0; i < 5; i++) {
        MyNode *now = addNode(QRandomGenerator::global()->bounded(-200, 200), QRandomGenerator::global()->bounded(-200, 200));
        if (i) addEdge(now, tmp[QRandomGenerator::global()->bounded(0, i)]);
        tmp.push_back(now);
    }

    connect(this, &MyScene::selectionChanged, this, &MyScene::updateStatusBar);
    connect(this, &MyScene::selectionChanged, &editWindow, &EditWindow::updateTable);

    switchMode(curMode);
}

void MyScene::updateStatusBar() {
    editWindow.updateTable();
    if (curMode == SelectMode) {
        QList <QGraphicsItem*> selected = selectedItems();
        int n = 0, e = 0;
        for (auto it: selected) {
            if (it->type() == MyNode::Type) ++n;
            if (it->type() == MyEdge::Type) ++e;
        }
        nodeCount->setText((n ? (QString::number(n) + " / ") : "") + QString::number(nodes.size()) + " Node" + (nodes.size() >= 2 ? "s" : ""));
        edgeCount->setText((e ? (QString::number(e) + " / ") : "") + QString::number(edges.size()) + " Edge" + (edges.size() >= 2 ? "s" : ""));
    }
    else {
        nodeCount->setText(QString::number(nodes.size()) + " Node" + (nodes.size() >= 2 ? "s" : ""));
        edgeCount->setText(QString::number(edges.size()) + " Edge" + (edges.size() >= 2 ? "s" : ""));
    }
    if (nodes.empty() && edges.empty())
        spStatus->setText("Empty graph");
    else if (edges.empty())
        spStatus->setText("Independent nodes");
    else if (!directed) {
        int fr = isForest();
        if (fr == 1) spStatus->setText("Tree");
        else if (fr) spStatus->setText("Forest with " + QString::number(fr) + " trees");
        else spStatus->setText("Undirected graph");
    }
    else {
        if (isDAG()) spStatus->setText("Directed acyclic graph");
        else spStatus->setText("Directed graph");
    }
}

MyNode* MyScene::addNode(qreal x, qreal y, QString name) {
    if (name.isNull()) {
        while (ids.count(QString::number(defaultNodeID))) ++defaultNodeID;
        name = QString::number(defaultNodeID);
    }
    if (ids.count(name)) return nullptr;

    MyNode* node = new MyNode(this, name);
    ids[name] = node;
    addItem(node);
    nodes.insert(node);
    node->setPos(x, y);
    updateStatusBar();
    return node;
}
MyEdge* MyScene::addEdge(MyNode *u, MyNode *v, QString weight) {
    MyEdge* edge = new MyEdge(this, u, v, weight);
    addItem(edge);
    edges.insert(edge);
    u->outEdge.insert(edge), v->inEdge.insert(edge);
    updateStatusBar();
    return edge;
}
void MyScene::delEdge(MyEdge *e) {
    removeItem(e);
    edges.erase(e);
    e->startNode->outEdge.erase(e);
    e->endNode->inEdge.erase(e);
    updateStatusBar();
    delete e;
}
void MyScene::delNode(MyNode *n) {
    removeItem(n);
    nodes.erase(n);
    ids.remove(n->name->text());
    std::vector <MyEdge*> es;
    for (auto e: n->outEdge) es.push_back(e);
    for (auto e: n->inEdge) es.push_back(e);
    n->inEdge.clear(), n->outEdge.clear();
    for (auto e: es) delEdge(e);
    updateStatusBar();
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

QPointF MyScene::randomNode() {
    QRect r = qgView->viewport()->rect();
    qDebug() << r << ' ';
    QPointF topLeft = qgView->mapToScene(r.topLeft()), bottomRight = qgView->mapToScene(r.bottomRight());
    QRectF R(topLeft, bottomRight);
    qDebug() << R << Qt::endl;
    if (R.width() > 200 && R.height() > 200) topLeft += QPointF(75, 75), bottomRight -= QPointF(75, 75);
    auto maxDis = [&](QPointF p) {
        qreal res = 1e9;
        for (auto n: nodes) {
            if (R.contains(n->scenePos()))
                res = std::min(res, QLineF(p, n->scenePos()).length());
        }
        return res;
    };

    QPointF res(QRandomGenerator::global()->bounded(bottomRight.x() - topLeft.x()) + topLeft.x(),
                QRandomGenerator::global()->bounded(bottomRight.y() - topLeft.y()) + topLeft.y());
    qreal score = maxDis(res);
    for (int _ = 1; _ <= 20; _++) {
        QPointF now(QRandomGenerator::global()->bounded(bottomRight.x() - topLeft.x()) + topLeft.x(),
                    QRandomGenerator::global()->bounded(bottomRight.y() - topLeft.y()) + topLeft.y());
        qreal _score = maxDis(now);
        if (_score > score) score = _score, res = now;
    }
    return res;
}

void MyScene::insertFromText(QString text) {
    randomNode();
    QStringList lines = text.split('\n', Qt::SkipEmptyParts);
    for (auto line: lines) {
        QStringList words = line.split(' ', Qt::SkipEmptyParts);
        if (words.size() <= 3) {
            QPointF rndNode = randomNode();
            addNode(rndNode.x(), rndNode.y(), words[0]);
            if (words.size() >= 2) {
                rndNode = randomNode();
                addNode(rndNode.x(), rndNode.y(), words[1]);
                addEdge(ids[words[0]], ids[words[1]], words.size() == 3 ? words[2] : nullptr);
            }
        }
    }
}

void MyScene::switchMode(CursorMode mode) {
    // enter new mode
    curMode = mode;
    updateStatusBar();
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

int MyScene::isForest() {
    QHash <MyNode*, bool> vis;
    QHash <MyEdge*, bool> visE;
    int ecnt = 0, ncnt = 0, ans = 0;
    for (auto n: nodes) vis[n] = 0;
    for (auto e: edges) visE[e] = 0;
    std::function<void(MyNode*)> dfs = [&](MyNode* x) {
        vis[x] = 1; ++ncnt;
        for (auto e: x->inEdge) {
            if (!visE[e]) visE[e] = 1, ++ecnt;
            visE[e] = 1;
            if (!vis[e->ad(x)]) dfs(e->ad(x));
        }
        for (auto e: x->outEdge) {
            if (!visE[e]) visE[e] = 1, ++ecnt;
            visE[e] = 1;
            if (!vis[e->ad(x)]) dfs(e->ad(x));
        }
    };
    for (auto n: nodes) if (!vis[n]) {
        int _e = ecnt, _n = ncnt;
        dfs(n);
        if (ecnt - _e == ncnt - _n - 1) ++ans;
        else return 0;
    }
    return ans;
}

bool MyScene::isDAG() {
    QHash <MyNode*, int> ind;
    for (auto e: edges) ++ind[e->endNode];
    QQueue <MyNode*> q;
    for (auto p: nodes) if (!ind[p]) q.push_back(p);
    while (!q.empty()) {
        MyNode *p = q.front(); q.pop_front();
        for (auto e: p->outEdge) {
            MyNode *x = e->ad(p);
            --ind[x]; if (!ind[x]) q.push_back(x);
        }
    }
    for (auto p: nodes) if (ind[p]) return 0;
    return 1;
}

void MyScene::toggleDirect() {
    directed ^= 1;
    for (auto e: edges) e->update();
    updateStatusBar();
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    if (event->button() & Qt::LeftButton) {

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
                addNode(clicked_point.x() - 25, clicked_point.y() - 25);
            }
        }
        dragged = 1;
        lastClickedPoint = event->scenePos();
        // qDebug() << "dragged " << lastClickedPoint << Qt::endl;
        increseBoundray();
    }
    if (curMode == SelectMode && event->button() == Qt::RightButton && !selectedItems().empty()) {
        // to not clear the current selection when right-clicked
        event->accept();
        return;
    }

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
            if (it->type() == MyNode::Type && it != HiddenNode) {
                want = it;
                break;
            }
        qDebug() << "!" << want << Qt::endl;
        if (want) {
            EndNode = qgraphicsitem_cast<MyNode*>(want);
            if (EndNode != DragNode) {
                addEdge(DragNode, EndNode);
            }
        }
        ADDedge = false;
        delete HiddenNode;
        delete myline;
        HiddenNode = nullptr, myline = nullptr;
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

void MyScene::nameNode(MyNode *n, QString name) {
    ids.remove(n->name->text());
    n->name->setText(name);
    ids[name] = n;
}
void MyScene::nameEdge(MyEdge *e, QString weight) {
    e->weight->setText(weight);
}

void MyScene::renameItem(QGraphicsItem *want) {
    bool ok = 0;
    QString text;
    if (want->type() == MyNode::Type) {
        auto it = qgraphicsitem_cast<MyNode*>(want);
        text = QInputDialog::getText(window, "Change name",
                                     "Changing node's name from " + it->name->text() + " to...", QLineEdit::Normal, QString(), &ok);
        if (ok) {
            if (!ids.count(text)) nameNode(it, text);
            else {
                QMessageBox::warning(window, "Name clashes", "The new name " + text + " has been occupied by another node.");
            }
        }
    }
    if (want->type() == MyEdge::Type) {
        auto it = qgraphicsitem_cast<MyEdge*>(want);
        text = QInputDialog::getText(window, "Change weight",
                                     "Changing edge's weight from " + it->weight->text() + " to...", QLineEdit::Normal, QString(), &ok);
        if (ok) nameEdge(it, text);
    }
    qDebug() << ok << ' ' << text << Qt::endl;
    editWindow.updateTable();
}

void MyScene::FRlayout(QList <QGraphicsItem*> items) {
    const double areaK = 40;

    if (items.empty()) return;
    QRectF bound;
    int nodeCnt = 0;
    for (auto it: items) if (it->type() == MyNode::Type) bound = it->sceneBoundingRect();
    for (auto it: items) if (it->type() == MyNode::Type) bound = bound.united(it->sceneBoundingRect()), nodeCnt++;
    if (!nodeCnt) return;

    bound = QRectF(bound.center(), bound.center());

    bound.adjust(-areaK * nodeCnt, -areaK * nodeCnt, areaK * nodeCnt, areaK * nodeCnt);
    setSceneRect(sceneRect().united(bound));
    const double area = bound.width() * bound.height();
    const double k = 1.2 * sqrt(area / nodeCnt);
    const double Itemp = bound.width() / 2;
    double temp = Itemp;
    const int numIt = 100;

    qDebug() << "auto layout: " << nodeCnt << ' ' << bound << ' ' << area << Qt::endl;

    auto f_a = [&](double d) { return 0.2 * d * d / k; }; // attractive force
    auto f_r = [&](double d) { return 0.2 * k * k / d; }; // repulsive force

    // QGraphicsRectItem *borderRect = new QGraphicsRectItem(bound);
    // QPen pen(Qt::red);
    // pen.setWidth(2);
    // borderRect->setPen(pen);
    // addItem(borderRect);

    for (auto _p: items) if (_p->type() == MyNode::Type) {
        QPointF pos(QRandomGenerator::global()->bounded(bound.width()) + bound.left(),
                    QRandomGenerator::global()->bounded(bound.height()) + bound.top());
        _p->setPos(pos);
    }
    for (int _ = 1; _ <= numIt; _++) {
        // bool ok; QInputDialog::getInt(window, "Set", "Sett", 1, -2147483647, 2147483647, 1, &ok);

        QHash <MyNode*, QPointF> delta;
        QHash <MyNode*, int> deg;
        for (auto _e: items) if (_e->type() == MyEdge::Type) {
            MyEdge* e = qgraphicsitem_cast<MyEdge*>(_e);
            QPointF d = e->endNode->scenePos() - e->startNode->scenePos();
            double len = e->line().length();
            if (len < 0.1) continue;
            ++deg[e->startNode], ++deg[e->endNode];
            delta[e->endNode] -= f_a(len) * d / len;
            delta[e->startNode] += f_a(len) * d / len;
        }
        for (auto _p: items) if (_p->type() == MyNode::Type) {
            MyNode *p = qgraphicsitem_cast<MyNode*>(_p);
            if (deg.count(p)) delta[p] /= sqrt(deg[p]);
        }

        for (int i = 0; i < items.size(); i++) if (items[i]->type() == MyNode::Type)
            for (int j = 0; j < i; j++) if (items[j]->type() == MyNode::Type) {
                MyNode *u = qgraphicsitem_cast<MyNode*>(items[i]), *v = qgraphicsitem_cast<MyNode*>(items[j]);
                QPointF d = v->scenePos() - u->scenePos();
                double len = QLineF(QPointF(), d).length();
                if (len < 0.1) continue;
                delta[v] += f_r(len) * d / len;
                delta[u] -= f_r(len) * d / len;
            }

        for (auto _p: items) if (_p->type() == MyNode::Type) {
            MyNode *p = qgraphicsitem_cast<MyNode*>(_p);
            QPointF d = delta[p];
            double len = QLineF(QPointF(), d).length();
            if (len < 0.1) continue;
            // qDebug() << p->name->text() << ' ' << d << ' ' << len << ' ' << temp << ' ' << p->scenePos() << Qt::endl;
            len = fmin(len, temp) / len; d.setX(d.x() * len), d.setY(d.y() * len);
            d += p->scenePos();
            // qDebug() << bound.left() << ' ' << bound.right() << ' ' << ' ' << bound.top() << ' ' << bound.bottom() << Qt::endl;
            d.setX(fmin(d.x(), bound.right())); d.setX(fmax(d.x(), bound.left()));
            d.setY(fmin(d.y(), bound.bottom())); d.setY(fmax(d.y(), bound.top()));
            p->setPos(d);
        }

        temp -= Itemp / numIt;
    }
    qgView->update();
}

void MyScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem * want = nullptr;
    QList <QGraphicsItem*> clicked_items = items(event->scenePos());
    for (auto it: clicked_items)
        if (it->type() == MyNode::Type || it->type() == MyEdge::Type) {
            want = it;
            break;
        }
    if (want && (curMode == MoveMode || curMode == SelectMode))
        renameItem(want);
    QGraphicsScene::mouseDoubleClickEvent(event);
}

template <typename T>
void shuffleQList (QList <T> &list) {
    if (list.empty()) return;
    for (int i = list.size() - 1; i >= 0; i--) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        list.swapItemsAt(i, j);
    }
}

void MyScene::linkChain(QList <QGraphicsItem*> items) {
    QHash <MyNode*, bool> have, vis;
    for (auto x: items)
        if (x->type() == MyNode::Type)
            have[qgraphicsitem_cast<MyNode*>(x)] = 1;
    int now = 0;
    QList <QList <MyNode*>> list;
    std::function<void(MyNode*)> dfs = [&](MyNode* x) {
        vis[x] = now; list[now - 1].push_back(x);
        for (auto e: x->outEdge) if (have[e->ad(x)] && !vis[e->ad(x)]) dfs(e->ad(x));
        for (auto e: x->inEdge) if (have[e->ad(x)] && !vis[e->ad(x)]) dfs(e->ad(x));
    };
    for (auto x: items)
        if (x->type() == MyNode::Type) {
            auto p = qgraphicsitem_cast<MyNode*>(x);
            if (!vis[p]) ++now, list.push_back({}), dfs(p);
        }
    qDebug() << list.size() << "!!" << Qt::endl;
    shuffleQList(list);
    for (auto &L: list) shuffleQList(L);
    for (int i = 1; i < list.size(); i++) {
        addEdge(list[i - 1][0], list[i][0]);
    }
}
void MyScene::linkTree(QList <QGraphicsItem*> items) {
    QHash <MyNode*, bool> have, vis;
    for (auto x: items)
        if (x->type() == MyNode::Type)
            have[qgraphicsitem_cast<MyNode*>(x)] = 1;
    int now = 0;
    QList <QList <MyNode*>> list;
    std::function<void(MyNode*)> dfs = [&](MyNode* x) {
        vis[x] = now; list[now - 1].push_back(x);
        for (auto e: x->outEdge) if (have[e->ad(x)] && !vis[e->ad(x)]) dfs(e->ad(x));
        for (auto e: x->inEdge) if (have[e->ad(x)] && !vis[e->ad(x)]) dfs(e->ad(x));
    };
    for (auto x: items)
        if (x->type() == MyNode::Type) {
            auto p = qgraphicsitem_cast<MyNode*>(x);
            if (!vis[p]) ++now, list.push_back({}), dfs(p);
        }
    qDebug() << list.size() << "!!" << Qt::endl;
    shuffleQList(list);
    for (auto &L: list) shuffleQList(L);
    for (int i = 1; i < list.size(); i++) {
        addEdge(list[QRandomGenerator::global()->bounded(i)][0], list[i][0]);
    }
}
void MyScene::linkComplete(QList <QGraphicsItem*> items) {
    QHash <QPair<MyNode*, MyNode*>, bool> vis;
    QList <MyNode*> ps;
    for (auto x: items)
        if (x->type() == MyNode::Type)
            ps.push_back(qgraphicsitem_cast<MyNode*>(x));
    for (auto e: edges) vis[{e->startNode, e->endNode}] = 1, vis[{e->endNode, e->startNode}] = 1;
    for (int i = 0; i < ps.size(); i++)
        for (int j = 0; j < i; j++) {
            if (!vis[{ps[i], ps[j]}]) {
                if (QRandomGenerator::global()->bounded(2))
                    addEdge(ps[i], ps[j]);
                else
                    addEdge(ps[j], ps[i]);
            }
        }
}

void MyScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    if (curMode != SelectMode || selectedItems().empty()) {
        QGraphicsItem * want = nullptr;
        QList <QGraphicsItem*> clicked_items = items(event->scenePos());
        for (auto it: clicked_items)
            if (it->type() == MyNode::Type || it->type() == MyEdge::Type) {
                want = it;
                break;
            }

        if (want) {
            QMenu menu;
            QAction *renameAction = menu.addAction("Rename");
            QAction *deleteAction = menu.addAction("Delete");
            QAction *act = menu.exec(event->screenPos());
            if (act == renameAction) {
                renameItem(want);
            }
            else if (act == deleteAction) {
                delItem(want);
            }
        }
    }
    else {
        QList <QGraphicsItem*> selected = selectedItems();
        QMenu menu;
        QAction *deleteAction = menu.addAction("Delete");
        QAction *deleteEdgeAction = menu.addAction("Delete edges");
        QAction *autoLayout = menu.addAction("Auto layout");
        QAction *selectionEdit = menu.addAction("Selection editor");
        QMenu *link = menu.addMenu("Randomly link as...");
        QAction *linkChainAction = link->addAction("Chain");
        QAction *linkTreeAction = link->addAction("Tree");
        QAction *linkCompleteAction = link->addAction("Complete Graph");

        QAction *act = menu.exec(event->screenPos());
        if (act == deleteAction) {
            for (auto x: selected) delItem(x);
        }
        if (act == deleteEdgeAction) {
            for (auto x: selected) if (x->type() == MyEdge::Type) delItem(x);
        }
        if (act == autoLayout) {
            FRlayout(selected);
        }
        if (act == selectionEdit) {
            editSelection();
        }
        if (act == linkChainAction) {
            linkChain(selected);
        }
        if (act == linkTreeAction) {
            linkTree(selected);
        }
        if (act == linkCompleteAction) {
            linkComplete(selected);
        }
    }
    QGraphicsScene::contextMenuEvent(event);
}

void MyScene::keyPressEvent(QKeyEvent *event) {
    if (curMode == SelectMode) {
        if (event->key() == Qt::Key_Delete) {
            QList <QGraphicsItem*> selected = selectedItems();
            std::sort(selected.begin(), selected.end(), [&](QGraphicsItem* u, QGraphicsItem *v) { return u->type() > v->type(); });
            for (auto x: selected) delItem(x);
        }
    }
    QGraphicsScene::keyPressEvent(event);
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

void MyScene::editSelection() {
    editWindow.show();
}
