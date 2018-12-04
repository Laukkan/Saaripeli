#include "hexitem.hh"
#include "constants.hh"

#include <QBrush>
#include <cmath>
#include <QMimeData>


namespace Student {


HexItem::HexItem(std::shared_ptr<Common::Hex> hex, QPointF center) :
    _size(SizeConstants::HEXSIZE), _hex(hex), _center(center)
{
    // Get the corners around _center and make the hex.
    QVector<QPointF> points = getHexCorners();
    setPolygon(QPolygonF(points));

    //  Set the color according to type.
    setBrush(ColorConstants::HEX_COLORS.at(_hex->getPieceType()));

    setAcceptedMouseButtons(Qt::RightButton);
    setAcceptDrops(true);
}

QPointF HexItem::pointyHexCorner(int side)
{
    double angle_deg = (60 * side) - 30;
    double angle_rad = (M_PI / 180) * angle_deg;

    return QPointF(_center.x() + ( _size * cos(angle_rad) ),
                   _center.y() + ( _size * sin(angle_rad) ));
}


QVector<QPointF> HexItem::getHexCorners()
{
    QVector<QPointF> points;
    int side = 0;

    const QSize pPixSize = SizeConstants::P_PIX_SIZE;
    const QSize aPixSize = SizeConstants::A_PIX_SIZE;

    while (side < OtherConstants::HEX_SIDES) {
        QPointF corner = pointyHexCorner(side);

        // Calculate the pawn and actor/transport positions
        if (side == 5) {
            _actorPosition = QPointF(corner.x() - aPixSize.width()/2,
                                     corner.y() + aPixSize.height()/14);
        }
        if (side == 1) {
            _pawnPositionMap[2] = QPointF(corner.x() - pPixSize.width(),
                                          corner.y() - pPixSize.height());
        }
        else if (side == 2)
        {
            _pawnPositionMap[1] = QPointF(corner.x() - pPixSize.width()/2,
                                          corner.y() - pPixSize.height());
            _transportPosition =  QPointF(corner.x() - aPixSize.width()/1.5,
                                          corner.y() - aPixSize.height()*1.25);
        }
        else if (side == 3) {
            _pawnPositionMap[3] = QPointF(corner.x(),
                                          corner.y() - pPixSize.height());

        }
        points.push_back(corner);
        ++side;
    }
    return points;
}

QPointF HexItem::getPawnPosition(int pawnId) const
{
    return _pawnPositionMap.at(pawnId);
}

QPointF HexItem::getActorPosition() const
{
    return _actorPosition;
}

QPointF HexItem::getTransportPosition() const
{
    return _transportPosition;
}


void HexItem::flip()
{
    // Fix HexItem color to match Water type
    setBrush(ColorConstants::HEX_COLORS.at("Water"));
    update();
}

void HexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    emit hexFlipped(_hex->getCoordinates());
}


void HexItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // Empty drags are ignored
    if (event->mimeData()->hasText()) {
        event->accept();
        update();
    }
    else {
        event->ignore();
    }

}

void HexItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    update();
}

void HexItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // Dropped PawnItem's/Actor's old parent (HexItem)
    // is the parent of the mimeData
    HexItem* oldParent = qobject_cast<HexItem*>(event->mimeData()->parent());

    // Do nothing when dropped on the same HexItem
    if (oldParent == this) {
        event->ignore();
        return;
    }
    event->accept();
    QStringList eventData = event->mimeData()->text().split(";");
    if(eventData.at(0) == "pawn") {
        emit pawnDropped(oldParent->_hex->getCoordinates(),
                         _hex->getCoordinates(),
                         eventData.at(1).toInt());
    }
    else if (eventData.at(0) == "actor") {
        emit actorDropped(oldParent->_hex->getCoordinates(),
                         _hex->getCoordinates(),
                         eventData.at(1).toInt());
    }
    else {
        emit transportDropped(oldParent->_hex->getCoordinates(),
                              _hex->getCoordinates(),
                              eventData.at(1).toInt());
    }
}

}
