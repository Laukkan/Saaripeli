#include "helpers.hh"
#include "constants.hh"
#include <math.h>
#include <map>


namespace Helpers {

QPointF cubeToPixel(const Common::CubeCoordinate &cubeCoord)
{
    unsigned int hexSize = SizeConstants::HEXSIZE;

    qreal q = cubeCoord.x;
    qreal r = cubeCoord.z;
    qreal x = hexSize * ( (sqrt(3) * q)  +  ((sqrt(3)/2) * r) );
    qreal y = hexSize * ( (3./2) * r );
    return QPointF(x, y);
}

QString gamePhaseToQString(const Common::GamePhase &gamePhase)
{
    if(gamePhase == 1){
        return "Movement";
    }
    else if (gamePhase == 2){
        return "Sinking";
    }
    else return "Spinning";
}

const QPixmap scaleActorImage(const QPixmap &pixmap, const int factor)
{
    return pixmap.scaled(SizeConstants::A_PIX_SIZE * factor,
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
}

}
