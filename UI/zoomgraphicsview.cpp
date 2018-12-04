#include "zoomgraphicsview.hh"
#include <QTimeLine>
#include <iostream>
#include "constants.hh"

ZoomGraphicsView::ZoomGraphicsView()
{
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ZoomGraphicsView::wheelEvent(QWheelEvent *event)
{
    // How much zooming with one mouse wheel tick
    static const double zoomFactor = 1.15;

    // Current zoom for checking boundaries
    static double currentZoom = 1.0;
    std::cout << currentZoom << std::endl;

    QPointF oldPos = mapToScene(event->pos());

    if (event->delta() > 0 && (currentZoom < OtherConstants::zoomMax)) {
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
