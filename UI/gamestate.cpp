#include "gamestate.hh"
#include "helpers.hh"

#include <random>
#include <time.h>


namespace Student {

GameState::GameState(const int playerAmount) :
    _currentPhase(Common::GamePhase::MOVEMENT)
{
    _currentPlayer = (Helpers::randomNumber(1, playerAmount));
}

Common::GamePhase GameState::currentGamePhase() const
{
    return _currentPhase;
}

int GameState::currentPlayer() const
{
    return _currentPlayer;
}

void GameState::changeGamePhase(Common::GamePhase nextPhase)
{
    _currentPhase = nextPhase;
}

void GameState::changePlayerTurn(int nextPlayer)
{
    _currentPlayer = nextPlayer;
}

}
