#include "hexitem.hh"
#include <QPainter>

#include <QRectF>
#include <cmath>

namespace Student {


// A constant map for determening the color of the hex depending on its type.
const static std::map<std::string,QColor> HEX_TYPES {
    {"Peak"    , QColor("darkGray")},
    {"Mountain", QColor("gray")},
    {"Forest"  , QColor("darkGreen")},
    {"Beach"   , QColor("yellow")},
    {"Water"   , QColor("cyan")},
    {"Coral"   , QColor("magenta")},
};


HexItem::HexItem(int size, std::shared_ptr<Common::Hex> hex, QPointF center) :
    _size(size),_hex(hex), _center(center)
{
    // Get the corners around _center and make the hex.
    QVector<QPointF> points = getHexCorners();
    setPolygon(QPolygonF(points));

    //  Set the color according to type.
    setBrush(HEX_TYPES.at(_hex->getPieceType()));
}


QPointF HexItem::pointyHexCorner(int side)
{
    double angle_deg = 60 * side - 30;
    double angle_rad = M_PI / 180 * angle_deg;

    return QPointF(_center.rx() + _size * cos(angle_rad),
                   _center.ry() + _size * sin(angle_rad));
}

QVector<QPointF> HexItem::getHexCorners()
{
    QVector<QPointF> points;
    int side = 0;

    while (side <= 5) {
        points.push_back(pointyHexCorner(side));
        side++;
    }
    return points;
}

void HexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Water-type doesn't interract
    if (_hex->isWaterTile()) {
        event->accept();
        return;
    }

    // Everything else sinks
    _hex->setPieceType("Water");
    setBrush(HEX_TYPES.at("Water"));
    update();
    event->accept();
}

}
