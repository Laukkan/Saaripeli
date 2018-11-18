#include "gameboard.hh"
#include "hexitem.hh"
#include "actor.hh"
#include "transport.hh"
#include <qmath.h>


GameBoard::GameBoard()
{

}

GameBoard::~GameBoard()
{

}

int GameBoard::checkTileOccupation(Common::CubeCoordinate tileCoord) const
{
    if(_hexes.find(tileCoord) == _hexes.end()){
        return -1;
    }
    else {
       return _hexes.at(tileCoord)->getPawnAmount();
    }
}

bool GameBoard::isWaterTile(Common::CubeCoordinate tileCoord) const
{

    if(_hexes.find(tileCoord) == _hexes.end()){
        return false;
    }
    else {
       return _hexes.at(tileCoord)->isWaterTile();
    }
}

std::shared_ptr<Common::Hex> GameBoard::getHex(Common::CubeCoordinate hexCoord) const
{
    if(_hexes.find(hexCoord) == _hexes.end()){
        return nullptr;
    }
    else {
       return _hexes.at(hexCoord);
    }
}

void GameBoard::addPawn(int playerId, int pawnId)
{
   std::shared_ptr<Common::Pawn> pawn(new Common::Pawn);
   pawn->setId(pawnId,playerId);
   _pawns[pawnId] = pawn;
}

void GameBoard::addPawn(int playerId, int pawnId, Common::CubeCoordinate coord)
{
    addPawn(playerId,pawnId);
    _pawns[pawnId]->setCoordinates(coord);

}

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{

    _pawns.at(pawnId)->setCoordinates(pawnCoord);
}

void GameBoard::removePawn(int pawnId)
{
    _pawns.erase(pawnId);
}

void GameBoard::addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord)
{
    _actors[actor->getId()] = actorCoord;
}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{
    _actors[actorId] = actorCoord;
}

void GameBoard::removeActor(int actorId)
{
    _actors.erase(actorId);
}

void GameBoard::addTransport(std::shared_ptr<Common::Transport> transport, Common::CubeCoordinate coord)
{
    _transports[transport->getId()] = transport;
    transport->addHex(_hexes[coord]);
}

void GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{
    _transports[id]->move(_hexes[coord]);
}

void GameBoard::removeTransport(int id)
{
    _transports.erase(id);
}

void GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    Common::CubeCoordinate newHexCoordinates = newHex->getCoordinates();
    _hexes[newHexCoordinates] = newHex;
}

/*void GameBoard::drawGameBoard(QGraphicsScene* scene)
{
    int rows = 9;
    int middleRow = rows-(rows-1)/2;
    int middleRowWidth = 9;
    for(int row=0; row < rows; row++){
        int numberOfHexes = middleRowWidth-abs((middleRow-1-row));
        int x = scene->width() / 2 -(numberOfHexes+0.5)/2*52;
        int y = scene->height() / 2 - (rows/2*46) + (row*46);

        Center center(x, y);

        drawRowOfHexes(scene,center,numberOfHexes);
    }
}

void GameBoard::drawRowOfHexes(QGraphicsScene* scene, Center rightMostCenter, int numberOfHexes)
{
    for(int hex=0; hex < numberOfHexes; hex++){
        HexItem* newHex = new HexItem(30, rightMostCenter);
        scene->addItem(newHex);
        rightMostCenter.x += 52;
    }
}*/

void GameBoard::drawGameBoard(QGraphicsScene* scene)
{
    for(auto hex = _hexes.begin(); hex != _hexes.end(); ++hex) {
        Common::CubeCoordinate cubeCoord = hex->first;
        {
            QPointF pointCenter = cube_to_pixel(cubeCoord);
            HexItem* newHex = new HexItem(30, pointCenter);
            scene->addItem(newHex);
        }


    }
}

QPointF GameBoard::cube_to_pixel(Common::CubeCoordinate cubeCoord)
{
    qreal q = cubeCoord.x;
    qreal r = cubeCoord.z;
    qreal x = 30 * (sqrt(3) * q  +  sqrt(3)/2 * r);
    qreal y = 30 * (3./2 * r);
    return QPointF(x, y);

}


