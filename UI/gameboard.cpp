#include "gameboard.hh"

#include "actor.hh"
#include "transport.hh"

#include <qmath.h>
#include <iterator>

namespace Student {

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

std::shared_ptr<Common::Hex> GameBoard::getHex(Common::CubeCoordinate hexCoord)
const
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
    std::shared_ptr<Common::Pawn> pawn(
                new Common::Pawn(pawnId, playerId, coord));
    _pawns[pawnId] = pawn;
    _pawns[pawnId]->setCoordinates(coord);
    _hexes[coord]->addPawn(pawn);
}

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{
    if (_hexes.find(pawnCoord) == _hexes.end()) {
        return;
    }
    std::shared_ptr<Common::Pawn> pawn = _pawns.at(pawnId);

    // Remove pawn from old coordinates and add to new
    _hexes.at(pawn->getCoordinates())->removePawn(pawn);
    _hexes.at(pawnCoord)->addPawn(pawn);

    pawn->setCoordinates(pawnCoord);
}

void GameBoard::removePawn(int pawnId)
{
    std::shared_ptr<Common::Pawn> pawn = _pawns.at(pawnId);

    // Remove from hex and map
    _hexes.at(pawn->getCoordinates())->removePawn(pawn);
    _pawns.erase(pawnId);
}

void GameBoard::addActor(
        std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord)
{
    _actors[actor->getId()] = actor;
    actor->move(_hexes[actorCoord]);
}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{
    if (_hexes.find(actorCoord) == _hexes.end()) {
        return;
    }
    _actors[actorId]->move(_hexes[actorCoord]);
}

void GameBoard::removeActor(int actorId)
{
    std::shared_ptr<Common::Actor> actor = _actors.at(actorId);

    // Remove from hex and map
    actor->getHex()->removeActor(actor);
    _actors.erase(actorId);
}

void GameBoard::addTransport(
        std::shared_ptr<Common::Transport> transport,
        Common::CubeCoordinate coord)
{
    _transports[transport->getId()] = transport;
    transport->addHex(_hexes[coord]);
}

void GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{
    if (_hexes.find(coord) == _hexes.end()) {
        return;
    }
    _transports[id]->move(_hexes[coord]);
}

void GameBoard::removeTransport(int id)
{
    std::shared_ptr<Common::Transport> transport = _transports.at(id);

    // Remove from hex and map
    transport->getHex()->removeTransport(transport);
    _transports.erase(id);
}

void GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    Common::CubeCoordinate newHexCoordinates = newHex->getCoordinates();
    _hexes[newHexCoordinates] = newHex;
}

std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>>
GameBoard::returnHexes()
{
    return _hexes;
}

bool GameBoard::checkIfActorOrTransportExists(std::string type)
{
    for(auto actor : _actors) {
        if(actor.second->getActorType() == type){
            return true;
        }
    }
    for(auto transport : _transports) {
        if(transport.second->getTransportType() == type){
            return true;
        }
    }
    return false;
}

std::shared_ptr<Common::Actor> GameBoard::getActor(int actorID)
{
    return _actors.at(actorID);
}

std::shared_ptr<Common::Transport> GameBoard::getTransport(int transportID)
{
    return _transports.at(transportID);
}

std::shared_ptr<Common::Pawn> GameBoard::getPawn(int pawnId)
{
    if (_pawns.find(pawnId) == _pawns.end()) {
        return nullptr;
    }

    return _pawns.at(pawnId);
}


int GameBoard::getWinner()
{
    if(_pawns.size() == 1){
        return _pawns.begin()->second->getPlayerId();
    }
    else {
        return 0;
    }
}

unsigned int GameBoard::pawnsLeft()
{
    return _pawns.size();
}

}
