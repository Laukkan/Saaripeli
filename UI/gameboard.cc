#include "gameboard.hh"
#include "hexitem.hh"
#include "actor.hh"
#include "transport.hh"
#include <qmath.h>


GameBoard::GameBoard(int hexSize):
    _hexSize(hexSize)
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

void GameBoard::drawGameBoard(QGraphicsScene* scene)
{
    for(auto hex = _hexes.begin(); hex != _hexes.end(); ++hex) {
        Common::CubeCoordinate cubeCoord = hex->first;
        {
            QPointF pointCenter = cube_to_pixel(cubeCoord);
            HexItem* newHex = new HexItem(_hexSize,
                                          hex->second->getPieceType(),
                                          pointCenter);
            scene->addItem(newHex);
        }
    }
}

QPointF GameBoard::cube_to_pixel(Common::CubeCoordinate cubeCoord)
{
    qreal q = cubeCoord.x;
    qreal r = cubeCoord.z;
    qreal x = _hexSize * (sqrt(3) * q  +  sqrt(3)/2 * r);
    qreal y = _hexSize * (3./2 * r);
    return QPointF(x, y);

}


