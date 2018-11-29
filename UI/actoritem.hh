#ifndef ACTORITEM_HH
#define ACTORITEM_HH

#include "actor.hh"
#include "hexitem.hh"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <memory>
namespace Student {

class ActorItem :   public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    ActorItem(std::shared_ptr<Common::Actor> actor, HexItem* parent);

public slots:
    void showActor();

protected:
    /**
     * @brief Interractions with mouse
     * @param event, Qt's mouse event class
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPixmap _actorImage;
    std::string _actorType;
    std::shared_ptr<Common::Actor> _actor;

};

}
#endif // ACTORITEM_HH
