#include "myview.h"

void MyView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        // zoom
        // const ViewportAnchor anchor = transformationAnchor();
        // setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        // int angle = event->angleDelta().y();
        // qreal factor;
        // if (angle > 0) {
        //     factor = 1.1;
        // } else {
        //     factor = 0.9;
        // }
        // scale(factor, factor);
        // setTransformationAnchor(anchor);
        int numDegrees = event->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // see QWheelEvent documentation
        _numScheduledScalings += numSteps;
        if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
            _numScheduledScalings = numSteps;

        QTimeLine *anim = new QTimeLine(350, this);
        anim->setUpdateInterval(20);

        connect(anim, &QTimeLine::valueChanged, this, &MyView::scalingTime);
        connect(anim, &QTimeLine::finished, this, &MyView::animFinished);
        anim->start();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void MyView::scalingTime(qreal x) {
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 300.0;
    factor = fmin(factor, 5 / curFactor), factor = fmax(factor, 0.2 / curFactor);
    curFactor *= factor;

    const ViewportAnchor anchor = transformationAnchor();
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale(factor, factor);
    setTransformationAnchor(anchor);

}

void MyView::animFinished() {
    if (_numScheduledScalings > 0)
        _numScheduledScalings--;
    else
        _numScheduledScalings++;
    sender()->~QObject();
}
