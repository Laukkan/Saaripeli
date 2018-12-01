#ifndef HELPERS_HH
#define HELPERS_HH

#include <cubecoordinate.hh>
#include <gamestate.hh>
#include <QString>
#include <QPointF>
#include <QPixmap>


namespace Helpers {


/**
 * @brief cube_to_pixel transforms cubeCoordinates into pixel coordinates.
 * The pixel coordinates depend on the hexes size.
 * @param cubeCoord The cube cordinates to be changed to pixel coordinates.
 * @return QPointF, cube coordinates changed in to pixel coordinates.
 */
QPointF cubeToPixel(const Common::CubeCoordinate &cubeCoord);

QString gamePhaseToQString(const Common::GamePhase &gamePhase);

const QPixmap scaleActorImage(const QPixmap &pixmap, const int factor);

}
#endif // HELPERS_HH
