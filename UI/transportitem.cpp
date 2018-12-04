#include "transportitem.hh"
#include "helpers.hh"
#include "constants.hh"

#include <QDrag>
#include <QCursor>
#include <QMimeData>
#include <QPointF>
#include <QPainter>

namespace Student {


TransportItem::TransportItem(std::shared_ptr<Common::Transport> transport,
                             HexItem* parent) :
    _transportType(transport->getTransportType()), _transport(transport)
{
    _transportImage.load(PathConstants::TRANSPORT_IMAGES.at(
                             _transport->getTransportType()));

    setPixmap(Helpers::scaleActorImage(_transportImage));
    QPointF coordinates = parent->getEmptyATPosition();
    setPos(coordinates);

    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setCursor(Qt::OpenHandCursor);
    setParent(parent);
}

void TransportItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::ClosedHandCursor);
}

void TransportItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::OpenHandCursor);
}

void TransportItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    QDrag* drag = new QDrag(parent());
    QMimeData* mime = new QMimeData;
    drag->setMimeData(mime);

    // Move information of the current parent and pawn Id
    mime->setParent(parent());
    mime->setText("transport;" + QString::number(_transport->getId()));

    drag->setPixmap(_transportImage);
    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

void TransportItem::switchTransportIcon(PawnItem* pawnItem)
{
    const std::map<std::string, QString> transportImages =
            PathConstants::TRANSPORT_IMAGES;

    if (_transportType == "dolphin")
    {
        _transportImage.load(
                    transportImages.at(_transportType +
                                       pawnItem->getColor().toStdString()));
        setPos(qobject_cast<HexItem*>(parent())->getFilledDolphinPosition());

    }
    // It's a boat
    else {
        if (_pawnItemsOnBoard.size() == 0) {
            _transportImage.load(
                        transportImages.at(_transportType +
                                           pawnItem->getColor().toStdString()));
        }
        else if (_pawnItemsOnBoard.size() == 2) {
            _transportImage.load(
                        transportImages.at(_transportType+"BlueWhiteRed"));
        }
        else {
            QString color1 = pawnItem->getColor();
            QString color2 = _pawnItemsOnBoard.at(0)->getColor();
            QString concat = color1 + color2;

            if (concat.contains("Blue") && concat.contains("Red"))
            {
                _transportImage.load(
                            transportImages.at(_transportType+"BlueRed"));
            }
            else if (concat.contains("Blue") && concat.contains("White"))
            {
                _transportImage.load(
                            transportImages.at(_transportType+"BlueWhite"));
            }
            else {
                _transportImage.load(
                            transportImages.at(_transportType+"WhiteRed"));
            }
        }
        setPos(qobject_cast<HexItem*>(parent())->getFilledBoatPosition());
    }
    setPixmap(Helpers::scaleActorImage(_transportImage));

    _pawnItemsOnBoard.push_back(pawnItem);
}

bool TransportItem::isABoat() const
{
    return _transportType == "boat";
}

void TransportItem::releasePawns()
{
    //removing the item from the children of the hex is needed for
    //repositioning hexitems
    HexItem* parentHex = qobject_cast<HexItem*>(parent());
    setParent(nullptr);

    for(PawnItem* pawnItem : _pawnItemsOnBoard){
        pawnItem->setOffset(parentHex->getPawnPosition(pawnItem->getId()));
        pawnItem->setParent(parentHex);
        pawnItem->show();
    }
     _pawnItemsOnBoard.clear();
}

}
