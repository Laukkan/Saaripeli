#ifndef HEXITEM_HH
#define HEXITEM_HH
#include "hex.hh"

#include <QGraphicsItem>
#include <memory>

namespace Student {

class HexItem : public QGraphicsPolygonItem
{
public:

    HexItem(int size, std::shared_ptr<Common::Hex> hex, QPointF center);

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
     * @brief type  The logical hex this hex is an item off.
     */
    std::shared_ptr<Common::Hex> _hex;

    /**
     * @brief _center The center of the hex (in the boards coordinates).
     */
    QPointF _center;

};

}

#endif // HEXITEM_HH
