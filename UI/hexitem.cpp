#include "hexitem.hh"
#include "helpers.hh"
#include "mainwindow.hh"
#include "illegalmoveexception.hh"
#include "constants.hh"

#include <QPainter>
#include <QRectF>
#include <cmath>
#include <QMimeData>


namespace Student {


HexItem::HexItem(int size, std::shared_ptr<Common::Hex> hex, QPointF center) :
    _size(size), _hex(hex), _center(center)
{
    // Get the corners around _center and make the hex.
    QVector<QPointF> points = getHexCorners();
    setPolygon(QPolygonF(points));

    //  Set the color according to type.
    setBrush(ColorConstants::HEX_COLORS.at(_hex->getPieceType()));

    _pawnPositionArray[0] = QPointF(_center.x()-8, _center.y());
    _pawnPositionArray[1] = QPointF(_center.x()-20, _center.y()-20);
    _pawnPositionArray[2] = QPointF(_center.x()+5, _center.y()-20);

    setAcceptedMouseButtons(Qt::LeftButton);
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

    while (side < SizeConstants::HEX_SIDES) {
        points.push_back(pointyHexCorner(side));
        side++;
    }
    return points;
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

QPointF HexItem::getPawnPosition()
{
    return _pawnPositionArray[children().size()];
}

QPointF HexItem::getActorPosition()
{
    return QPointF(_center.x()-15, _center.y()-23);
}

void HexItem::flip()
{
    // Fix HexItem color to match Water type
    setBrush(ColorConstants::HEX_COLORS.at("Water"));
    update();
}

}
