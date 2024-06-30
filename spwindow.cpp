#include "spwindow.h"

#include "myscene.h"
#include "mainwindow.h"

SPWindow::SPWindow(MyScene *scene, QWidget *parent)
    : QWidget{parent}, scene(scene)
{
    setWindowTitle("Shortest path results (row x, col y: x -> y)");
    setGeometry(100, 100, 600, 400);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(&tableWidget);
    setLayout(mainLayout);

    tableWidget.setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget.setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(&tableWidget, &QTableWidget::cellClicked, this, &SPWindow::selectCell);
}

void SPWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    hide();
}

bool SPWindow::updateSP(QList <QGraphicsItem*> items) {
    for (auto x: items) {
        if (x->type() == MyNode::Type) V.push_back(qgraphicsitem_cast<MyNode*>(x));
        if (x->type() == MyEdge::Type) E.push_back(qgraphicsitem_cast<MyEdge*>(x));
    }
    QHash <MyNode*, bool> tmp;
    for (auto e: E) tmp[e->startNode] = tmp[e->endNode] = 1;
    for (auto p: V) tmp[p] = 1;
    V.clear();
    for (auto it = tmp.constBegin(); it != tmp.constEnd(); ++it) V.push_back(it.key());
    std::sort(V.begin(), V.end(), [&](MyNode *u, MyNode *v) { return u->name->text() < v->name->text(); });
    for (auto e: E) {
        if (e->weight->text().isNull() || e->weight->text().isEmpty()) continue;
        bool ok = 0;
        e->weight->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(scene->window, "Error", "Edge " + e->startNode->name->text() + " ---(" + e->weight->text() + ")---> " + e->endNode->name->text() + " has non-number weight");
            return 0;
        }
    }

    std::function<bool(MyNode*)> bellman_ford = [&](MyNode *S) {
        QHash <MyNode*, double> dis;
        dis[S] = 0;
        QList <MyNode*> upd;
        for (int _ = 0; _ < 2 * V.size(); _++) {
            if (_ < V.size()) upd.clear();
            for (auto e: E) {
                if (!dis.count(e->startNode)) continue;
                double w = (e->weight->text().isNull() || e->weight->text().isEmpty()) ? 1 : e->weight->text().toDouble();
                if (!dis.count(e->endNode) || dis[e->endNode] > dis[e->startNode] + w){
                    if (!first.count({S, e->endNode})) first[{S, e->endNode}] = e;
                    dis[e->endNode] = dis[e->startNode] + w;
                    from[{S, e->endNode}] = e;
                    upd.push_back(e->endNode);
                }
            }
            if (!scene->directed) {
                for (auto e: E) {
                    if (!dis.count(e->endNode)) continue;
                    double w = (e->weight->text().isNull() || e->weight->text().isEmpty()) ? 1 : e->weight->text().toDouble();
                    if (!dis.count(e->startNode) || dis[e->startNode] > dis[e->endNode] + w){
                        if (!first.count({S, e->startNode})) first[{S, e->startNode}] = e;
                        dis[e->startNode] = dis[e->endNode] + w;
                        from[{S, e->startNode}] = e;
                        upd.push_back(e->startNode);
                    }
                }
            }
            if (upd.empty()) break;
        }
        for (auto p: V) ans[{S, p}] = dis.count(p) ? QString::number(dis[p]) : "NaN";
        for (auto p: upd) ans[{S, p}] = "-inf";
        return !upd.empty();
    };
    for (auto p: V) {
        if (bellman_ford(p)) {
            // QMessageBox::warning(this, "Error", "Negative cycle found");
            // return;
            qDebug() << "Negative cycle found from " << p->name->text() << Qt::endl;
        }
    }
    tableWidget.setRowCount(V.size());
    tableWidget.setColumnCount(V.size());
    QStringList headers;
    for (auto p: V) headers << p->name->text();
    tableWidget.setVerticalHeaderLabels(headers);
    tableWidget.setHorizontalHeaderLabels(headers);
    tableWidget.verticalHeader()->setSectionsClickable(0);
    tableWidget.horizontalHeader()->setSectionsClickable(0);
    tableWidget.setCornerButtonEnabled(0);
    for (int i = 0; i < V.size(); i++) for (int j = 0; j < V.size(); j++) {
            QString s = ans[{V[i], V[j]}];
            QTableWidgetItem *item = new QTableWidgetItem(s);
            item->setTextAlignment(Qt::AlignCenter);
            tableWidget.setItem(i, j, item);
        }
    return 1;
}

void SPWindow::selectCell(int r, int c) {
    if (ans[{V[r], V[c]}] == "NaN") return;
    qDebug("%d %d\n", r, c);
    QHash <MyNode*, bool> vis; bool turn = 0;
    MyNode *s = V[r], *e = V[c];
    scene->clearSelection();
    while (!( (ans[{V[r], V[c]}] != "-inf" || turn) && e == s )) {
        if (vis[e]) turn = 1;
        vis[e] = 1;
        qDebug() << e << ' ' << e->name->text() << Qt::endl;
        e->setSelected(1);
        auto edge = turn ? first[{s, e}] : from[{s, e}];
        qDebug() << "Edge" << ' ' << edge->startNode->name->text() << ' ' << edge->endNode->name->text() << ' ' << edge->weight->text() << Qt::endl;
        edge->setSelected(1);
        edge->setZValue(scene->standard_z); e->setZValue(scene->standard_z);
        e = edge->ad(e);
    }
    s->setZValue(scene->standard_z); s->setSelected(1);
    ++scene->standard_z;
}
