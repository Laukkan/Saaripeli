#ifndef ACTORITEM_HH
#define ACTORITEM_HH

#include "actor.hh"

#include <QGraphicsPixmapItem>
#include <memory>

class ActorItem :   public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    ActorItem(std::string actorType, std::shared_ptr<Common::Hex> hex);

public slots:
    void showActor();

private:
    QPixmap _actorImage;
    std::string _actorType;
    std::shared_ptr<Common::Hex> _hex;
};

#endif // ACTORITEM_HH
