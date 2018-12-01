#include "transportitem.hh"

#include "helpers.hh"

#include <QDrag>
#include <QCursor>
#include <QMimeData>
#include <QPointF>
#include <QPainter>

namespace Student {


TransportItem::TransportItem(std::shared_ptr<Common::Transport> transport, HexItem* parent) :
    _transport(transport)
{

    _transportImage.load(TRANSPORT_TYPES.at(_transport->getTransportType()));
    setPixmap(_transportImage.scaled(30,46));
    QPointF coordinates = parent->getActorPosition();
    setPos(coordinates);

    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setCursor(Qt::OpenHandCursor);
    setParent(parent);

}

void TransportItem::showTransport()
{
    this->show();
}

void TransportItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::ClosedHandCursor);
}

void TransportItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::OpenHandCursor);
}

void TransportItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    QDrag* drag = new QDrag(parent());
    QMimeData* mime = new QMimeData;
    drag->setMimeData(mime);

    // Move information of the current parent and pawn Id
    mime->setParent(parent());
    mime->setText("transport;" + QString::number(_transport->getId()));

    drag->setPixmap(_transportImage);
    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

}
