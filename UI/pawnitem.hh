#ifndef PAWNITEM_HH
#define PAWNITEM_HH

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include "pawn.hh"
#include "hexitem.hh"
#include "player.hh"

namespace Student {

class PawnItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit PawnItem(std::shared_ptr<Player> player,std::shared_ptr<Common::Pawn> pawn, HexItem* parent);

    std::shared_ptr<Common::Pawn> returnPawn();

protected:
    /**
     * @brief Interractions with mouse
     * @param event, Qt's mouse event class
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPixmap _pawnImage;
    std::shared_ptr<Common::Pawn> _pawn;
    std::shared_ptr<Player> _player;
};

}

#endif // PAWNITEM_HH
