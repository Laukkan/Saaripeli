#include "helpers.hh"
#include <math.h>


namespace Helpers {


QPointF cubeToPixel(Common::CubeCoordinate cubeCoord)
{
    qreal q = cubeCoord.x;
    qreal r = cubeCoord.z;
    qreal x = HEXSIZE * ( (sqrt(3) * q)  +  ((sqrt(3)/2) * r) );
    qreal y = HEXSIZE * ( (3./2) * r );
    return QPointF(x, y);
}

}
