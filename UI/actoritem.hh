#ifndef ACTORITEM_HH
#define ACTORITEM_HH

#include "actor.hh"
#include "hexitem.hh"

#include <QGraphicsPixmapItem>
#include <memory>


namespace Student {

class ActorItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    /**
     * @brief ActorItem - Constructor for an ActorItem, a graphicalItem used
     * to indicate a logical actor.
     * @param actor - Pointer to the actor the actorItem portrays.
     * @param parent - HexItem*, pointer to hex that the actorItem is on.
     */
    ActorItem(std::shared_ptr<Common::Actor> actor, HexItem* parent);

protected:
    /**
     * @brief Interractions with mouse
     * @param event, Qt's mouse event class
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    /**
     * @brief _actorImage stores the actors icon.
     */
    QPixmap _actorImage;

    /**
     * @brief _actor corresponding gamelogic counterpart of this actorItem.
     */
    std::shared_ptr<Common::Actor> _actor;

};

}
#endif // ACTORITEM_HH
