#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QTimeLine>

class MyView : public QGraphicsView
{
public:
    MyView (QWidget *parent = nullptr): QGraphicsView(parent) {};
    MyView (QGraphicsScene *scene, QWidget *parent = nullptr): QGraphicsView(scene, parent) {};

protected:
    void wheelEvent(QWheelEvent *event) override;
private:
    int _numScheduledScalings = 0;
    double curFactor = 1;
private slots:
    void scalingTime(qreal x);
    void animFinished();
};

#endif // MYVIEW_H
