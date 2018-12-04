#include "helpers.hh"
#include "constants.hh"
#include <sstream>


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
    else {
        return "Spinning";
    }
}

const QPixmap scaleActorImage(const QPixmap &pixmap, const int factor)
{
    return pixmap.scaled(SizeConstants::A_PIX_SIZE * factor,
                         Qt::IgnoreAspectRatio,
                         Qt::SmoothTransformation);
}

int randomNumber(const int min, const int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(gen);
}


std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}



}
