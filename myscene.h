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

    MyNode* addNode(qreal x, qreal y);
    MyEdge* addEdge(MyNode *u, MyNode *v);


private:
    bool dragged;
    QGraphicsView *qgView;
    void increseBoundray();
    int boundrayWidth;
    int extendAmount;
    QPointF lastClickedPoint;

    float standard_z = 1e-5;
};

#endif // MYSCENE_H
