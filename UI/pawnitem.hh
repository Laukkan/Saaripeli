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
    explicit PawnItem(QString color,std::shared_ptr<Common::Pawn> pawn, HexItem* parent);

    QString getColor();

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
    QString _color;
};

}

#endif // PAWNITEM_HH
