#ifndef PAWNITEM_HH
#define PAWNITEM_HH

#include "pawn.hh"
#include "hexitem.hh"
#include "player.hh"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>


namespace Student {

class PawnItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:

    /**
     * @brief PawnItem's constructor. Constructs the image of the PawnItem,
     * sets the PawnItem to the board.
     * @param color - the color of the pawn
     * @param pawn - corresponding logical pawn
     * @param parent - the HexItem the PawnItem resides in
     */
    explicit PawnItem(QString color,
                      std::shared_ptr<Common::Pawn> pawn,
                      HexItem* parent);

    // A virtual destructor is already provided by the QObject subclass.

     QString getColor();

     /**
      * @brief getId - returns the logical pawn id
      * @return - id (1-3)
      */
     int getId() const;

protected:
    /**
     * @brief PawnItem's Interractions with mouse
     * @param event, Qt's mouse event class
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    /**
     * @brief _pawnImage - the pixmap image of the pawn
     */
    QPixmap _pawnImage;
    /**
     * @brief _pawn - A pointer to the GameEngine side logical pawn
     */
    std::shared_ptr<Common::Pawn> _pawn;
    /**
     * @brief _color - QString that indicates the pawns color.
     */
    QString _color;
};

}

#endif // PAWNITEM_HH
