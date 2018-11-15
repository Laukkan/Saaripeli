#ifndef HEXITEM_HH
#define HEXITEM_HH
#include <QGraphicsItem>
#include <renderarea.hh>


class HexItem : public QGraphicsItem
{
public:
    HexItem(int size, Center center);
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr);
    QRectF boundingRect() const;
    QPointF pointyHexCorner(int side);

private:
    int _size;
    Center _center;

};

#endif // HEXITEM_HH
