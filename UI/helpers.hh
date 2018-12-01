#ifndef HELPERS_HH
#define HELPERS_HH

#include <cubecoordinate.hh>
#include <gamestate.hh>
#include <QString>
#include <QPointF>


namespace Helpers {


/**
 * @brief cube_to_pixel transforms cubeCoordinates into pixel coordinates.
 * The pixel coordinates depend on the hexes size.
 * @param cubeCoord The cube cordinates to be changed to pixel coordinates.
 * @return QPointF, cube coordinates changed in to pixel coordinates.
 */
QPointF cubeToPixel(Common::CubeCoordinate cubeCoord);

QString gamePhaseToQString(Common::GamePhase gamePhase);

}
#endif // HELPERS_HH
