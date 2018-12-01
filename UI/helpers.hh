#ifndef HELPERS_HH
#define HELPERS_HH

#include <cubecoordinate.hh>
#include <gamestate.hh>
#include <QString>
#include <QPointF>
#include <map>

// Distance from each hexes corner to their middle.
const static int HEXSIZE = 30;

const static std::map<std::string,QString> ACTOR_TYPES {
    {"shark"    , ":/shark.png"},
    {"kraken"    , ":/kraken.png"},
    {"seamunster"    , ":/seamunster.png"},
    {"vortex"    , ":/vortex.png"},
};

const static std::map<std::string,QString> TRANSPORT_TYPES {
    {"dolphin"    , ":/dolphin.png"},
    {"boat"    , ":/boat.png"},
};

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
