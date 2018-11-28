#include "actoritem.hh"

#include "actor.hh"
#include "hex.hh"
#include "helpers.hh"

#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QPainter>
namespace Student {

const static std::map<std::string,QString> ACTOR_TYPES {
    {"shark"    , ":/shark.png"},
};

ActorItem::ActorItem(std::string actorType, std::shared_ptr<Common::Hex> hex) :
    _actorType(actorType), _hex(hex)
{
    _actorImage.load(ACTOR_TYPES.at(_actorType));
    setPixmap(_actorImage.scaled(30,46));
    QPointF coordinates = Helpers::cubeToPixel(_hex->getCoordinates());
    setPos(QPointF(coordinates.x()-15,coordinates.y()-23));
}

void ActorItem::showActor()
{
    this->show();
}

}
