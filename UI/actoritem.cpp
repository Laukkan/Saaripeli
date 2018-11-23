#include "actoritem.hh"

#include "actor.hh"
#include "hex.hh"
#include "helpers.hh"

#include <QGraphicsPixmapItem>
#include <QPointF>

const static std::map<std::string,std::string> ACTOR_TYPES {
    {"shark"    , ":/shark.jpg"},
};

ActorItem::ActorItem(std::string actorType, std::shared_ptr<Common::Hex> hex) :
    _actorType(actorType), _hex(hex)
{
    _actorImage.load(":/shark.jpg");
    setPixmap(_actorImage.scaled(30,46));
    setOffset(Helpers::cubeToPixel(_hex->getCoordinates()));
}
