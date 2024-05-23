#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "mynode.h"
#include "myedge.h"

class MyScene : public QGraphicsScene
{
    Q_OBJECT

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
public:
    explicit MyScene(QObject *parent = nullptr, QGraphicsView *q = nullptr);

    MyNode* addNode(qreal x, qreal y, QString _name = nullptr);
    MyEdge* addEdge(MyNode *u, MyNode *v, QString _weight = nullptr);
    void delEdge(MyEdge *);
    void delNode(MyNode *);
    void delItem(QGraphicsItem *);

    enum CursorMode { MoveMode, SelectMode, AddMode, DeleteMode } curMode = MoveMode;
    void switchMode(CursorMode mode);
    bool directed = 1;
    void toggleDirect();

private:
    QMap <QString, MyNode*> ids;
    std::set <MyNode*> nodes;
    std::set <MyEdge*> edges;

    bool dragged;
    QGraphicsView *qgView;
    void increseBoundray();
    int boundrayWidth;
    int extendAmount;
    QPointF lastClickedPoint;

    qreal standard_z = 1e-5;
    int defaultNodeID = 0;
};

#endif // MYSCENE_H
