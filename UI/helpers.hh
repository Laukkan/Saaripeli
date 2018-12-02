#ifndef HELPERS_HH
#define HELPERS_HH

#include <cubecoordinate.hh>
#include <gamestate.hh>
#include <QString>
#include <QPointF>
#include <QPixmap>
#include <vector>


using PixmapIter = std::vector<QPixmap>::iterator;

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
const QPixmap scaleActorImage(const QPixmap &pixmap, const int factor);

/**
 * @brief - A function that returns a random image from a given Pixmap
 * vector
 */
QPixmap selectRandomImage(PixmapIter start, PixmapIter end,
                          std::default_random_engine &randomEng);

}
#endif // HELPERS_HH
