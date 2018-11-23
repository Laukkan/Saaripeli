#ifndef ACTORITEM_HH
#define ACTORITEM_HH

#include "actor.hh"

#include <QGraphicsPixmapItem>
#include <memory>

class ActorItem : public QGraphicsPixmapItem
{
public:
    ActorItem(std::string actorType, std::shared_ptr<Common::Hex> hex);

private:
    QPixmap _actorImage;
    std::string _actorType;
    std::shared_ptr<Common::Hex> _hex;
};

#endif // ACTORITEM_HH
