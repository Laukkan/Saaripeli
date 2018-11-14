#include "gameboard.hh"


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

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{

    _pawns.at(pawnId)->setCoordinates(pawnCoord);
}

void GameBoard::removePawn(int pawnId)
{
    _pawns.erase(pawnId);
}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{
    _actors[actorId] = actorCoord;
}

void GameBoard::removeActor(int actorId)
{
    _actors.erase(actorId);
}

void GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    Common::CubeCoordinate newHexCoordinates = newHex->getCoordinates();
    _hexes[newHexCoordinates] = newHex;
}
