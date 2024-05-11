#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "mynode.h"

class MyScene : public QGraphicsScene
{
    Q_OBJECT

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
public:
    explicit MyScene(QObject *parent = nullptr);

    void addNode(qreal x, qreal y);

private:
    bool pressing;
    QPointF startPointer, startCenter;
};

#endif // MYSCENE_H
