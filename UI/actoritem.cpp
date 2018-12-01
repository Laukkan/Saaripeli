#include "actoritem.hh"
#include "helpers.hh"
#include "constants.hh"

#include <QDrag>
#include <QCursor>
#include <QMimeData>
#include <QPointF>
#include <QPainter>
#include <QSize>


namespace Student {


ActorItem::ActorItem(std::shared_ptr<Common::Actor> actor, HexItem* parent) :
    _actor(actor)
{
    _actorImage.load(PathConstants::ACTOR_IMAGES.at(_actor->getActorType()));
    setPixmap(_actorImage.scaled(SizeConstants::A_PIX_SIZE));
    QPointF coordinates = parent->getActorPosition();
    setPos(coordinates);

    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setCursor(Qt::OpenHandCursor);
    setParent(parent);

}

void ActorItem::showActor()
{
    this->show();
}

void ActorItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::ClosedHandCursor);
}

void ActorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::OpenHandCursor);
}

void ActorItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    QDrag* drag = new QDrag(parent());
    QMimeData* mime = new QMimeData;
    drag->setMimeData(mime);

    // Move information of the current parent and the actor
    mime->setParent(parent());
    mime->setText("actor;" + QString::number(_actor->getId()));

    QPixmap dragPixmap = _actorImage.scaled(SizeConstants::A_PIX_SIZE * 2,
                                            Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation);
    drag->setPixmap(dragPixmap);
    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

}

