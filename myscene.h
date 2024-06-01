#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QLabel>
#include <QKeyEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>

#include "mynode.h"
#include "myedge.h"

class MainWindow;

class MyScene : public QGraphicsScene
{
    Q_OBJECT

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyPressEvent(QKeyEvent *keyEvent) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
public:
    explicit MyScene(QObject *parent = nullptr, QGraphicsView *q = nullptr, QLabel *_node = nullptr, QLabel *_edge = nullptr, QLabel *_sp = nullptr);

    MyNode* addNode(qreal x, qreal y, QString _name = nullptr);
    MyEdge* addEdge(MyNode *u, MyNode *v, QString _weight = nullptr);
    void delEdge(MyEdge *);
    void delNode(MyNode *);
    void nameNode(MyNode *, QString);
    void nameEdge(MyEdge *, QString);
    void delItem(QGraphicsItem *);
    void renameItem(QGraphicsItem *);

    void insertFromText(QString);

    enum CursorMode { MoveMode, SelectMode, AddMode, DeleteMode } curMode = MoveMode;
    void switchMode(CursorMode mode);
    bool directed = 1;
    void toggleDirect();
    MyNode * DragNode = nullptr;
    MyNode * EndNode = nullptr;
    bool ADDedge=false;
    MyEdge * myline = nullptr;
    MyNode * HiddenNode = nullptr;

    void updateStatusBar();

private:
    int isForest();

    QMap <QString, MyNode*> ids;
    std::set <MyNode*> nodes;
    std::set <MyEdge*> edges;

    bool dragged;
    QGraphicsView *qgView;
    void increseBoundray();
    const int boundrayWidth = 10;
    const int extendAmount = 1000;
    QPointF lastClickedPoint;

    qreal standard_z = 1e-5;
    int defaultNodeID = 1;

    QLabel *nodeCount, *edgeCount, *spStatus;
    friend class MainWindow;
    MainWindow *window;

    QPointF randomNode();
};

#endif // MYSCENE_H
