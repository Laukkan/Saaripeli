#include "pawnitem.hh"

namespace Student {

PawnItem::PawnItem(std::shared_ptr<Common::Pawn> pawn):
    _pawn(pawn)
{
    _pawnImage = QPixmap("./Assets/pawn.jpg");
    setPixmap(_pawnImage);
    setOffset(cubeToPixel(_pawn->getCoordinates()));
}

QPointF PawnItem::cubeToPixel(Common::CubeCoordinate cubeCoord)
{
    qreal q = cubeCoord.x;
    qreal r = cubeCoord.z;
    qreal x = 30 * (sqrt(3) * q  +  sqrt(3)/2 * r);
    qreal y = 30 * (3./2 * r);
    return QPointF(x, y);

}

}
