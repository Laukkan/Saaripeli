#ifndef HEXITEM_HH
#define HEXITEM_HH
#include <QGraphicsItem>


class HexItem : public QGraphicsPolygonItem
{
public:
    HexItem(int size, QPointF center);
    QPointF pointyHexCorner(int side);
    QVector<QPointF> getHexCorners();

private:
    int _size;
    QPointF _center;

};

#endif // HEXITEM_HH
