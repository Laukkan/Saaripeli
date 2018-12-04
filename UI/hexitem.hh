#ifndef HEXITEM_HH
#define HEXITEM_HH
#include "hex.hh"

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <memory>


namespace Student {

class HexItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    /**
     * @brief HexItem's constructor
     * @param hex - the corresponding GameEngine side's Hex
     * @param center - the center coordinate of the HexItems location
     */
    explicit HexItem(std::shared_ptr<Common::Hex> hex, QPointF center);

    // A virtual destructor is already provided by the QObject subclass.

    /**
     * @brief getPawnPosition - returns a position for a pawn based on it's id
     * @param pawnId - the id of the pawn (1-3)
     * @return the QPointF coordinate representation of the position.
     */
    QPointF getPawnPosition(int pawnId) const;

    /**
     * @brief getActorPosition - returns the position for the ActorItem based
     * on the HexItem's location.
     * @return QPointF pixel coordinate representation of the position.
     */
    QPointF getActorPosition() const;
    QPointF getTransportPosition() const;

    /**
     * @brief flip - flips the HexItem upside down sinking it.
     * @details is called by the MainWindow's flipHex slot if the flip of
     * the Hex is legal (according to the rules).
     */
    void flip();

signals:
    /**
     * @brief pawn-/actor-/transportDropped - signals for each of the possible
     * dropEvent's for the HexItem
     * @param origin - this hex's CubeCoordinate position
     * @param target - target hex's CubeCoordinate position
     * @param pawn-/actor/transportId - what is getting moved
     */
    void pawnDropped(Common::CubeCoordinate origin,
                     Common::CubeCoordinate target,
                     int pawnId);
    void actorDropped(Common::CubeCoordinate origin,
                     Common::CubeCoordinate target,
                     int actorId);
    void transportDropped(Common::CubeCoordinate origin,
                     Common::CubeCoordinate target,
                     int transportId);

    /**
     * @brief hexFlipped - signal that is emitted every time the HexItem is
     * pressed.
     * @param tileCoord - the hex's CubeCoordinate position
     */
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
     * @brief _size Distance from any corner to the middle of the hex in pixels
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

    /**
     * @brief _aTPosition - position for empty transport and actor
     */
    QPointF _actorPosition;

    /**
     * @brief Positions for filled transports
     */
    QPointF _transportPosition;

    /**
     * @brief _pawnPositionMap - contains the positions for each of the
     * 3 pawns based on their Id
     */
    std::map<int, QPointF> _pawnPositionMap;

    /**
     * @brief getHexCorners Returns a vector with all of the hexes corners and
     * calcutates the points for Pawns and Actor/Transport
     * @return QVector<QPointF> , a vector with all of the hexes corners.
     */
    QVector<QPointF> getHexCorners();

    /**
     * @brief pointyHexCorner calculates and returns the QPointF representation
     * of the given side's cornerpoint.
     * @param side (0-5) the number of the side
     * @return QPointF the coordinate of the corner
     * @details is looped from 0 to 5 by the getHexCorners method.
     */
    QPointF pointyHexCorner(int side);
};

}

#endif // HEXITEM_HH
