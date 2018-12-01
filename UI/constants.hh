#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <QSize>
#include <QColor>
#include <map>


namespace SizeConstants {

// ActorItem's pixmap's picture dimensions
const static QSize A_PIX_SIZE = QSize(30, 46);

// PawnItem's pixmap's picture dimensions
const static QSize P_PIX_SIZE = QSize(15, 23);

// GameInfoBox's size
const static QSize INFO_BOX_SIZE = QSize(100, 200);

// Distance from each hexes corner to their middle.
const static unsigned int HEXSIZE = 30;

const static int HEX_SIDES = 6;

}


namespace PathConstants {

// Map of paths for the each of the colored pawn images
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
    {"dolphin",  ":/dolphin.png"},
    {"boat",     ":/boat.png"},
    {"boatBlue", ":/boatBlue.png"},
    {"boatWhite", ":/boatWhite.png"},
    {"boatRed", ":/boatRed.png"},
    {"boatBlueRed", ":/boatBlueRed.png"},
    {"boatBlueWhite", ":/boatBlueWhite.png"},
    {"boatBlueWhiteRed", ":/boatBlueWhiteRed.png"},
    {"boatWhiteRed", ":/boatWhiteRed.png"},
    {"dolphinBlue",  ":/dolphinBlue.png"},
    {"dolphinRed",  ":/dolphinRed.png"},
    {"dolphinWhite",  ":/dolphinWhite.png"}

};

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
