#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include "igamestate.hh"

#include <QSize>
#include <QColor>
#include <QPoint>
#include <map>


namespace OtherConstants {

// Position offset for the GameInfoBox in the MainWindow
const static QPoint GIBOX_OFFSET = QPoint(500, -400);

// Shuffle animation time (milliseconds) for the GameInfoBox
const static unsigned ANIM_TIME = 1000;

const static int HEX_SIDES = 6;

// zoom limits for ZoomGraphicsView
const static double ZOOM_MIN = 0.5;
const static double ZOOM_MAX = 6;

// The delimiter used in the ranking file.
const static char DELIMITER = ';';

}

namespace GameConstants {

const static int MIN_PLAYERS = 2;
const static int MAX_PLAYERS = 3;

const static int POINTS_FOR_WIN = 3;


// Used to determine the next GamePhase from a GamePhase
const static std::map<Common::GamePhase, Common::GamePhase> NEXT_GAME_PHASE {
    {Common::GamePhase::MOVEMENT, Common::GamePhase::SINKING},
    {Common::GamePhase::SINKING, Common::GamePhase::SPINNING},
    {Common::GamePhase::SPINNING, Common::GamePhase::MOVEMENT}
};

}

namespace SizeConstants {

// The size of the MainWindow
const static QSize MW_SIZE = QSize(1280,720);

// ActorItem's pixmap's picture dimensions
const static QSize A_PIX_SIZE = QSize(30, 46);

// PawnItem's pixmap's picture dimensions
const static QSize P_PIX_SIZE = QSize(12, 19);

// GameInfoBox's size
const static QSize INFO_BOX_SIZE = QSize(100, 200);

// Distance from each hexes corner to their middle.
const static unsigned int HEXSIZE = 50;



}

namespace PathConstants {

// Movable Actor's images
const static std::map<std::string, QString> M_ACTOR_IMAGES {
    {"shark",      ":/shark.png"},
    {"kraken",     ":/kraken.png"},
    {"seamunster", ":/seamunster.png"},
    {"dolphin",    ":/dolphin.png"},
};

const static std::map<QString, QString> PAWN_IMAGES {
    {"White", ":/whitepawn.png"},
    {"Blue",  ":/bluepawn.png"},
    {"Red",   ":/redpawn.png"},
};

const static std::map<std::string, QString> ACTOR_IMAGES {
    {"shark",      ":/shark.png"},
    {"kraken",     ":/kraken.png"},
    {"seamunster", ":/seamunster.png"},
    {"vortex",     ":/vortex.png"},
};

const static std::map<std::string, QString> TRANSPORT_IMAGES {
    {"dolphin",          ":/dolphin.png"},
    {"boat",             ":/boat.png"},
    {"boatBlue",         ":/boatBlue.png"},
    {"boatWhite",        ":/boatWhite.png"},
    {"boatRed",          ":/boatRed.png"},
    {"boatBlueRed",      ":/boatBlueRed.png"},
    {"boatBlueWhite",    ":/boatBlueWhite.png"},
    {"boatBlueWhiteRed", ":/boatBlueWhiteRed.png"},
    {"boatWhiteRed",     ":/boatWhiteRed.png"},
    {"dolphinBlue",      ":/dolphinBlue.png"},
    {"dolphinRed",       ":/dolphinRed.png"},
    {"dolphinWhite",     ":/dolphinWhite.png"}

};

//const static std::string RANKING_FILE = "C:/Users/Eeru/opisto_ohjelmointi/OTEK/kansakas/UI/ranking.csv";
const static std::string RANKING_FILE = "ranking.csv";
}

namespace ColorConstants {

// A map for determening the color of the hex depending on its type.
const static std::map<std::string, QColor> HEX_COLORS {
    {"Peak"    , QColor("darkGray")},
    {"Mountain", QColor("gray")},
    {"Forest"  , QColor("darkGreen")},
    {"Beach"   , QColor("yellow")},
    {"Water"   , QColor("cyan")},
    {"Coral"   , QColor("magenta")},

};

// A Map for each of the Pawn's colors
const static std::map<int, QString> PAWN_COLORS {
    {1    , "White"},
    {2    , "Blue"},
    {3    , "Red"}
};

}

#endif // CONSTANTS_HH
