#include "zoomgraphicsview.hh"
#include <QTimeLine>
#include "constants.hh"

ZoomGraphicsView::ZoomGraphicsView()
{
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ZoomGraphicsView::wheelEvent(QWheelEvent *event)
{
    // How much zooming with one mouse wheel tick
    static const double zoomFactor = 1.15;

    static double currentZoom = 1.0;

    QPointF oldPos = mapToScene(event->pos());

    if (event->delta() > 0 && (currentZoom < OtherConstants::ZOOM_MAX)) {
        scale(zoomFactor, zoomFactor);
        currentZoom *= zoomFactor;
    } else if (currentZoom > OtherConstants::ZOOM_MIN) {
        scale(1 / zoomFactor, 1 / zoomFactor);
        currentZoom /= zoomFactor;
    }
    QPointF newPos = mapToScene(event->pos());
    QPointF delta = newPos - oldPos;
    translate(delta.x(), delta.y());

}
