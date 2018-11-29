#ifndef HEXITEM_HH
#define HEXITEM_HH
#include "hex.hh"

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <memory>
#include <array>



namespace Student {

class HexItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:

    HexItem(int size, std::shared_ptr<Common::Hex> hex, QPointF center);



    /**
     * @brief getHexCorners Returns a vector with all of the hexes corners.
     * @return QVector<QPointF> , a vector with all of the hexes corners.
     */

    QPointF getPawnPosition();
    QPointF getActorPosition();

    void flip();

signals:
    void pawnDropped(Common::CubeCoordinate origin,
                     Common::CubeCoordinate target,
                     int pawnId);
    void actorDropped(Common::CubeCoordinate origin,
                     Common::CubeCoordinate target,
                     int actorId);
    void hexFlipped(Common::CubeCoordinate tileCoord);

protected:
    /**
     * @brief HexItems interractions with mouse clicks and drops
     * @param event, Qt's mouse event class
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * event) override;
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent * event) override;
    virtual void dropEvent(QGraphicsSceneDragDropEvent * event) override;

private:

    /**
     * @brief _size Distance from any corne-r to the middle of the hex in pixels;
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

    QPointF _pawnPositionArray[3];

    /**
     * @brief getHexCorners Returns a vector with all of the hexes corners.
     * @return QVector<QPointF> , a vector with all of the hexes corners.
     */
    QVector<QPointF> getHexCorners();
    QPointF pointyHexCorner(int side);
};

}

#endif // HEXITEM_HH
