#include "actoritem.hh"

#include "actor.hh"
#include "hex.hh"
#include "helpers.hh"

#include <QDrag>
#include <QCursor>
#include <QMimeData>
#include <QPointF>
#include <QPainter>

namespace Student {


ActorItem::ActorItem(std::shared_ptr<Common::Actor> actor, HexItem* parent) :
    _actor(actor)
{

    _actorImage.load(ACTOR_TYPES.at(_actor->getActorType()));
    setPixmap(_actorImage.scaled(30,46));
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

    // Move information of the current parent and pawn Id
    mime->setParent(parent());
    mime->setText("actor;" + QString::number(_actor->getId()));

    drag->setPixmap(_actorImage);
    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

}

