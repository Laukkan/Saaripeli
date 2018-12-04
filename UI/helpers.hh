#ifndef HELPERS_HH
#define HELPERS_HH

#include <cubecoordinate.hh>
#include <gamestate.hh>

#include <QString>
#include <QPointF>
#include <QPixmap>
#include <vector>
#include <random>


namespace Helpers {

/**
 * @brief cube_to_pixel transforms cubeCoordinates into pixel coordinates.
 * The pixel coordinates depend on the hexes size.
 * @param cubeCoord The cube cordinates to be changed to pixel coordinates.
 * @return QPointF, cube coordinates changed in to pixel coordinates.
 */
QPointF cubeToPixel(const Common::CubeCoordinate &cubeCoord);

/**
 * @brief gamePhaseToQString - transforms a given GamePhase to a Qstring and
 * returns it
 * @param gamePhase - the given Common::GamePhase
 * @return - the transformation result as QString
 */
QString gamePhaseToQString(const Common::GamePhase &gamePhase);

/**
 * @brief scaleActorImage - scales the given Actor pixmap by the factor
 * @param pixmap - Actor image/pixmap
 * @param factor - the factor the scale done with.
 * @return a constant scaled pixmap
 */
const QPixmap scaleActorImage(const QPixmap &pixmap, const int factor = 1);

/**
 * @brief randomNumber - returns a random number between the interval defined
 * by the parameters (min <= return value <= max)
 * @param min - min of the interval
 * @param max - max of the interval
 * @return - a random number between this interval
 */
int randomNumber(const int min, const int max);

std::vector<std::string> split(const std::string& s, char delimiter);

}
#endif // HELPERS_HH
