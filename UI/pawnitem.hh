#ifndef PAWNITEM_HH
#define PAWNITEM_HH

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include "pawn.hh"

namespace Student {

class PawnItem : public QGraphicsPixmapItem
{
public:
    explicit PawnItem(std::shared_ptr<Common::Pawn> pawn);

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
};

}

#endif // PAWNITEM_HH
