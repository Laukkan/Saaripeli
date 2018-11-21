#include "pawnitem.hh"
#include "helpers.hh"

namespace Student {

PawnItem::PawnItem(std::shared_ptr<Common::Pawn> pawn):
    _pawn(pawn)
{
    _pawnImage = QPixmap("./Assets/pawn.jpg");
    setPixmap(_pawnImage);
    setOffset(Helpers::cubeToPixel(_pawn->getCoordinates()));
}

QPointF PawnItem::cubeToPixel(Common::CubeCoordinate cubeCoord)
{ 
    qreal q = cubeCoord.x;
    qreal r = cubeCoord.z;
    qreal x = HEXSIZE * ( (sqrt(3) * q)  +  ((sqrt(3)/2) * r) );
    qreal y = HEXSIZE * ( (3/2) * r );
    return QPointF(x, y);
}

}
