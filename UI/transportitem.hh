#ifndef TRANSPORTITEM_HH
#define TRANSPORTITEM_HH

#include "transport.hh"
#include "hexitem.hh"
#include "pawnitem.hh"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <memory>
namespace Student {

class TransportItem:   public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    TransportItem(std::shared_ptr<Common::Transport> transport, HexItem* parent);
    void switchTransportIcon(PawnItem* pawnItem);

public slots:
    void showTransport();

protected:
    /**
     * @brief Interractions with mouse
     * @param event, Qt's mouse event class
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPixmap _transportImage;
    std::string _transportType;
    std::shared_ptr<Common::Transport> _transport;
    std::vector<PawnItem*> _pawnItemsOnBoard;

};

}

#endif // TRANSPORTITEM_HH
