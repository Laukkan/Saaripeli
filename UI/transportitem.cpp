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

    setPixmap(_transportImage.scaled(30,46));
    QPointF coordinates = parent->getActorPosition();
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
    }
    else{
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
            std::vector<QString> colors;
            colors.push_back(pawnItem->getColor());
            colors.push_back(_pawnItemsOnBoard.at(0)->getColor());
            if(std::find(colors.begin(), colors.end(), "Red") != colors.end()
                    and std::find(colors.begin(), colors.end(), "Blue") != colors.end()){
                _transportImage.load(
                            transportImages.at(_transportType+"BlueRed"));
            }
            else if(std::find(colors.begin(), colors.end(), "White") != colors.end()
                    and std::find(colors.begin(), colors.end(), "Blue") != colors.end()){
                _transportImage.load(
                            transportImages.at(_transportType+"BlueWhite"));
            }
            else {
                _transportImage.load(
                            transportImages.at(_transportType+"WhiteRed"));
            }
        }
    }
    setPixmap(_transportImage.scaled(SizeConstants::A_PIX_SIZE));
    _pawnItemsOnBoard.push_back(pawnItem);
}

}
