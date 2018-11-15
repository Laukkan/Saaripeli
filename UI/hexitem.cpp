#include "hexitem.hh"
#include <QPainter>
#include <QRectF>
#include <cmath>

HexItem::HexItem(int size, Center center) :
    _size(size), _center(center)
{

}

void HexItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
       QPointF points[6];
       int side = 0;

       while (side <= 5) {
           points[side] = pointyHexCorner(side);
           side++;
       }
       painter->drawPolygon(points, 6);
}

QPointF HexItem::pointyHexCorner(int side)
{
    double angle_deg = 60 * side - 30;
    double angle_rad = M_PI / 180 * angle_deg;

    return QPointF(_center.x + _size * cos(angle_rad),
                  _center.y + _size * sin(angle_rad));
}

QRectF HexItem::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-_size+penWidth/2,-_size+penWidth/2,2*_size,2*_size);
}
