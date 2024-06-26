#include "editwindow.h"

#include "myscene.h"

EditWindow::EditWindow(MyScene *scene, QWidget *parent)
    : QWidget{parent}, scene(scene)
{
    setWindowTitle("Console (Right-click to edit)");
    setGeometry(100, 100, 600, 400);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    tableWidget = new ConsoleTable(this);
    tableWidget->setColumnCount(4);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList headers;
    headers << "" << "Start Node" << "End Node" << "Weight";
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->verticalHeader()->setVisible(false);
    updateTable();

    connect(tableWidget, &QTableWidget::itemClicked, this, &EditWindow::toggleSelection);
    connect(tableWidget, &QTableWidget::itemChanged, this, &EditWindow::editItem);
    tableWidget->resizeColumnToContents(0);

    mainLayout->addWidget(tableWidget);

    setLayout(mainLayout);
}

void EditWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    hide();
}

void EditWindow::updateTable() {
    userEditing = 0;
    items.clear();
    QHash <QGraphicsItem*, bool> vis;
    for (auto x: scene->selectedItems()) vis[x] = 1;
    for (auto p: scene->nodes) items.push_back(p);
    for (auto e: scene->edges) items.push_back(e);
    std::sort(items.begin(), items.end(), [&](QGraphicsItem *u, QGraphicsItem *v) {
        if (vis[u] != vis[v]) return vis[u];
        if (u->type() != v->type()) return u->type() == MyNode::Type;
        if (u->type() == MyNode::Type) {
            auto U = qgraphicsitem_cast<MyNode*>(u), V = qgraphicsitem_cast<MyNode*>(v);
            return U->name->text() < V->name->text();
        }
        else {
            auto U = qgraphicsitem_cast<MyEdge*>(u), V = qgraphicsitem_cast<MyEdge*>(v);
            if (U->startNode->name->text() != V->startNode->name->text())
                return U->startNode->name->text() < V->startNode->name->text();
            return U->endNode->name->text() < V->endNode->name->text();
        }
    });

    tableWidget->setRowCount(items.size());

    for (int row = 0; row < items.size(); ++row) {
        QString s[3];
        if (items[row]->type() == MyNode::Type)
            s[0] = qgraphicsitem_cast<MyNode*>(items[row])->name->text();
        else {
            auto e = qgraphicsitem_cast<MyEdge*>(items[row]);
            s[0] = e->startNode->name->text();
            s[1] = e->endNode->name->text();
            s[2] = e->weight->text();
        }
        for (int col = 1; col < 4; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(s[col - 1]);
            item->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, col, item);
        }

        QCheckBox *checkBox = new QCheckBox();
        checkBox->setStyleSheet("QCheckBox::indicator {"
                                "    subcontrol-origin: padding;"
                                "    subcontrol-position: center;"
                                "}");
        checkBox->setChecked(vis[items[row]]);
        tableWidget->setCellWidget(row, 0, checkBox);
    }
    userEditing = 1;
}

void EditWindow::toggleSelection(QTableWidgetItem *item) {
    qDebug() << item << Qt::endl;
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(tableWidget->cellWidget(item->row(), 0));
    items[item->row()]->setSelected(!checkBox->isChecked());
    updateTable();
}

void EditWindow::editItem(QTableWidgetItem *item) {
    if (!userEditing) return;
    auto it = items[item->row()];
    if (it->type() == MyNode::Type) {
        if (item->column() == 1) {
            auto p = qgraphicsitem_cast<MyNode*>(it);
            scene->nameNode(p, item->text());
        }
        else item->setText("");
    }
    else {
        auto e = qgraphicsitem_cast<MyEdge*>(it);
        if (item->column() == 1) {
            if (scene->ids.count(item->text()))
                scene->edgeChangeStart(e, scene->ids[item->text()]);
            else
                item->setText(e->startNode->name->text());
        }
        if (item->column() == 2) {
            if (scene->ids.count(item->text()))
                scene->edgeChangeEnd(e, scene->ids[item->text()]);
            else
                item->setText(e->endNode->name->text());
        }
        if (item->column() == 3) {
            scene->nameEdge(e, item->text());
        }
    }
}
