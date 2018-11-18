#include "hexitem.hh"
#include <QPainter>

#include <QRectF>
#include <cmath>

HexItem::HexItem(int size, std::string type, QPointF center) :
    _size(size), _center(center), _type(type)
{
    QVector<QPointF> points = getHexCorners();
    setPolygon(QPolygonF(points));
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

