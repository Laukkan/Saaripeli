#ifndef TRANSPORTITEM_HH
#define TRANSPORTITEM_HH

#include "transport.hh"
#include "hexitem.hh"
#include "pawnitem.hh"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <memory>


namespace Student {

class TransportItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    /**
     * @brief TransportItem's constructor. Initialises the image, position
     * and the parent HexItem.
     * @param transport - the logical transport this transport is item off
     * @param parent - the parent HexItem this TransportItem resides in
     */
    explicit TransportItem(std::shared_ptr<Common::Transport> transport,
                           HexItem* parent);

    // A virtual destructor is already provided by the QObject subclass.

    /**
     * @brief switchTransportIcon - switches to an according TransportIcon
     * based on what's already on board and who's added.
     * @param pawnItem - the new Pawnitem to be added
     */
    void switchTransportIcon(PawnItem* pawnItem);

    /**
     * @brief releasePawns - releases all of the PawnItems from the Transport
     * Item
     */
    void releasePawns();

    /**
     * @brief isABoat - Returns true if the transportItem is a boat
     * @return true or false
     */
    bool isABoat() const;

protected:
    /**
     * @brief TransportItems's interractions with mouse
     * @param event, Qt's mouse event class
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    /**
     * @brief _transportImage - the image of TransportItem
     */
    QPixmap _transportImage;
    std::string _transportType;
    /**
     * @brief _transport - the corresponding GameEngine's logical transport
     */
    std::shared_ptr<Common::Transport> _transport;
    std::vector<PawnItem*> _pawnItemsOnBoard;
};

}

#endif // TRANSPORTITEM_HH
