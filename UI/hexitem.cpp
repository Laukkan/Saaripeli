#include "hexitem.hh"
#include "helpers.hh"
#include "mainwindow.hh"
#include "illegalmoveexception.hh"

#include <QPainter>
#include <QRectF>
#include <cmath>
#include <QMimeData>


const static int HEX_SIDES = 6;

// A constant map for determening the color of the hex depending on its type.
const static std::map<std::string,QColor> HEX_TYPES {
    {"Peak"    , QColor("darkGray")},
    {"Mountain", QColor("gray")},
    {"Forest"  , QColor("darkGreen")},
    {"Beach"   , QColor("yellow")},
    {"Water"   , QColor("cyan")},
    {"Coral"   , QColor("magenta")},
};

namespace Student {


HexItem::HexItem(int size, std::shared_ptr<Common::Hex> hex, QPointF center) :
    _size(size), _hex(hex), _center(center)
{
    // Get the corners around _center and make the hex.
    QVector<QPointF> points = getHexCorners();
    setPolygon(QPolygonF(points));

    //  Set the color according to type.
    setBrush(HEX_TYPES.at(_hex->getPieceType()));

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

    while (side < HEX_SIDES) {
        points.push_back(pointyHexCorner(side));
        side++;
    }
    return points;
}

void HexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    try {
        emit hexFlipped(_hex->getCoordinates());
    }
    catch (Common::IllegalMoveException) {
        event->ignore();
        return;
    }

    // Fix HexItem color to match Water type
    event->accept();
    setBrush(HEX_TYPES.at("Water"));
    update();
    emit turned();
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
    // Dropped PawnItem's old parent (HexItem) is the parent of the mimeData
    HexItem* oldParent = qobject_cast<HexItem*>(event->mimeData()->parent());

    // Do nothing when dropped on the same HexItem
    if (oldParent == this) {
        event->ignore();
        return;
    }
    event->accept();
    int pawnId = event->mimeData()->text().toInt();
    emit pawnDropped(oldParent->_hex->getCoordinates(),
                     _hex->getCoordinates(),
                     pawnId);

}

QPointF HexItem::getPawnPosition()
{
    return _pawnPositionArray[children().size()];
}

}
