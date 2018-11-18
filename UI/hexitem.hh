#ifndef HEXITEM_HH
#define HEXITEM_HH
#include <QGraphicsItem>


class HexItem : public QGraphicsPolygonItem
{
public:

    HexItem(int size, std::string type, QPointF center);

    /**
     * @brief pointyHexCorner Gives a QPointF to each corner of the hex d
     * depending on the side parameter.
     * @param side int, determines which corner is given. Value between 0-5.
     * @return QPointF, the location of the corner.
     */
    QPointF pointyHexCorner(int side);

    /**
     * @brief getHexCorners Returns a vector with all of the hexes corners.
     * @return QVector<QPointF> , a vector with all of the hexes corners.
     */
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
