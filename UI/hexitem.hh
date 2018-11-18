#ifndef HEXITEM_HH
#define HEXITEM_HH
#include <QGraphicsItem>


class HexItem : public QGraphicsPolygonItem
{
public:
    /**
     * @brief HexItem
     * @param size
     * @param type
     * @param center
     */
    HexItem(int size, std::string type, QPointF center);
    QPointF pointyHexCorner(int side);
    QVector<QPointF> getHexCorners();

private:

    /**
     * @brief _size Distance from any corner to the middle of the hex in pixels;
     */
    int _size;

    /**
     * @brief type  The hexes type (Mountain,Forest,etc), used to determine the color of the hex.
     */
    std::string _type;

    /**
     * @brief _center The center of the hex (in the boards coordinates).
     */
    QPointF _center;

};

#endif // HEXITEM_HH
