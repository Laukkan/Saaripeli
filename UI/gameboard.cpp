#include "gameboard.hh"
#include "hexitem.hh"
#include "actor.hh"
#include "transport.hh"

#include <qmath.h>
#include <iterator>


Student::GameBoard::GameBoard(int hexSize):
    _hexSize(hexSize)
{

}

Student::GameBoard::~GameBoard()
{

}

int Student::GameBoard::checkTileOccupation(Common::CubeCoordinate tileCoord) const
{
    if(_hexes.find(tileCoord) == _hexes.end()){
        return -1;
    }
    else {
       return _hexes.at(tileCoord)->getPawnAmount();
    }
}

bool Student::GameBoard::isWaterTile(Common::CubeCoordinate tileCoord) const
{

    if(_hexes.find(tileCoord) == _hexes.end()){
        return false;
    }
    else {
       return _hexes.at(tileCoord)->isWaterTile();
    }
}

std::shared_ptr<Common::Hex> Student::GameBoard::getHex(Common::CubeCoordinate hexCoord) const
{
    if(_hexes.find(hexCoord) == _hexes.end()){
        return nullptr;
    }
    else {
       return _hexes.at(hexCoord);
    }
}

void Student::GameBoard::addPawn(int playerId, int pawnId)
{
   std::shared_ptr<Common::Pawn> pawn(new Common::Pawn);
   pawn->setId(pawnId,playerId);
   _pawns[pawnId] = pawn;
}

void Student::GameBoard::addPawn(int playerId, int pawnId, Common::CubeCoordinate coord)
{
    std::shared_ptr<Common::Pawn> pawn(new Common::Pawn);
    pawn->setId(pawnId,playerId);
    _pawns[pawnId] = pawn;
    _pawns[pawnId]->setCoordinates(coord);
    _hexes[coord]->addPawn(pawn);

}

void Student::GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{

    _pawns.at(pawnId)->setCoordinates(pawnCoord);
}

void Student::GameBoard::removePawn(int pawnId)
{
    _pawns.erase(pawnId);
}

void Student::GameBoard::addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord)
{
    _actors[actor->getId()] = actor;
    actor->move(_hexes[actorCoord]);
}

void Student::GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{
    _actors[actorId]->move(_hexes[actorCoord]);
}

void Student::GameBoard::removeActor(int actorId)
{
    _actors.erase(actorId);
}

void Student::GameBoard::addTransport(std::shared_ptr<Common::Transport> transport, Common::CubeCoordinate coord)
{
    _transports[transport->getId()] = transport;
    transport->addHex(_hexes[coord]);
}

void Student::GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{
    _transports[id]->move(_hexes[coord]);
}

void Student::GameBoard::removeTransport(int id)
{
    _transports.erase(id);
}

void Student::GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    Common::CubeCoordinate newHexCoordinates = newHex->getCoordinates();
    _hexes[newHexCoordinates] = newHex;
}

void Student::GameBoard::drawGameBoard(QGraphicsScene* scene)
{
    for(auto hex = _hexes.begin(); hex != _hexes.end(); ++hex) {
        Common::CubeCoordinate cubeCoord = hex->first;
        {
            QPointF pointCenter = cubeToPixel(cubeCoord);
            HexItem* newHex = new HexItem(_hexSize,
                                          hex->second->getPieceType(),
                                          pointCenter);

            scene->addItem(newHex);
        }
    }
}

QPointF Student::GameBoard::cubeToPixel(Common::CubeCoordinate cubeCoord)
{
    qreal q = cubeCoord.x;
    qreal r = cubeCoord.z;
    qreal x = _hexSize * (sqrt(3) * q  +  sqrt(3)/2 * r);
    qreal y = _hexSize * (3./2 * r);
    return QPointF(x, y);

}


