#ifndef PAWNITEM_HH
#define PAWNITEM_HH

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "pawn.hh"

namespace Student {

class PawnItem : public QGraphicsPixmapItem
{
public:

    PawnItem(std::shared_ptr<Common::Pawn> pawn);

    /**
     * @brief cube_to_pixel transforms cubeCoordinates into pixel coordinates.
     * The pixel coordinates depend on the hexes size.
     * @param cubeCoord The cube cordinates to be changed to pixel coordinates.
     * @return QPointF, cube coordinates changed in to pixel coordinates.
     */
    QPointF cubeToPixel(Common::CubeCoordinate cubeCoord);

private:
    QPixmap _pawnImage;
    std::shared_ptr<Common::Pawn> _pawn;
};

}

#endif // PAWNITEM_HH
