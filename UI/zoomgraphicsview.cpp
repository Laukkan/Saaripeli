#include "zoomgraphicsview.hh"
#include <QTimeLine>
#include <iostream>
#include "constants.hh"

ZoomGraphicsView::ZoomGraphicsView(QMainWindow* parent)
{
    setParent(parent);
    //_currentZoom = 1.0;
}

/*void ZoomGraphicsView::scalingTime()
{
    qreal factor = 1.0+ qreal(_scheduledScalings) / 300.0;
    std::cout << _currentZoom << std::endl;
    if(_currentZoom < OtherConstants::zoomMax and _currentZoom > OtherConstants::zoomMin){
        scale(factor, factor);
        _currentZoom *= factor;
    }
    else if(_currentZoom > OtherConstants::zoomMin){
        scale(1/factor,1/factor);
        _currentZoom /= factor;
    }
}

void ZoomGraphicsView::animationFinsihed()
{
    if (_scheduledScalings > 0){
        _scheduledScalings--;
    }
    else {
         _scheduledScalings++;
         sender()->~QObject();
    }
}

void ZoomGraphicsView::wheelEvent(QWheelEvent *event)
{
    QPointF oldPos = mapToScene(event->pos());
    int degrees = event->delta() / 8;
    int steps = degrees / 15; // see QWheelEvent documentation
    _scheduledScalings += steps;
    if (_scheduledScalings * steps < 0){ // if user moved the wheel in another direction, we reset previously scheduled scalings
        _scheduledScalings = steps;
    }

    QTimeLine* animation = new QTimeLine(350, this);
    animation->setUpdateInterval(20);

    connect(animation, &QTimeLine::valueChanged, this, &ZoomGraphicsView::scalingTime);
    connect(animation, &QTimeLine::finished, this, &ZoomGraphicsView::animationFinsihed);
    animation->start();
    QPointF newPos = mapToScene(event->pos());
    QPointF delta = newPos - oldPos;
    translate(delta.x(), delta.y());
}*/

void ZoomGraphicsView::wheelEvent(QWheelEvent *event)
{
    // How much zooming with one mouse wheel tick
    static const double zoomFactor = 1.15;

    // Current zoom for checking boundaries
    static double currentZoom = 1.0;
    std::cout << currentZoom << std::endl;

    QPointF oldPos = mapToScene(event->pos());

    if (event->delta() > 0 && currentZoom < OtherConstants::zoomMax) {
        scale(zoomFactor, zoomFactor);
        currentZoom *= zoomFactor;
    } else if (currentZoom > OtherConstants::zoomMin) {
        scale(1 / zoomFactor, 1 / zoomFactor);
        currentZoom /= zoomFactor;
    }
    QPointF newPos = mapToScene(event->pos());
    QPointF delta = newPos - oldPos;
    translate(delta.x(), delta.y());

}
