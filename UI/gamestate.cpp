#include "gamestate.hh"
#include "helpers.hh"
#include "constants.hh"

#include <random>


namespace Student {

GameState::GameState(const int playerAmount) :
    _currentPhase(Common::GamePhase::MOVEMENT),
    _playerAmount(playerAmount)

{
    _currentPlayer = Helpers::randomNumber(1, _playerAmount);
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
    // Not the next gamePhase
    if (GameConstants::NEXT_GAME_PHASE.at(_currentPhase) != nextPhase) {
        return;
    }
    _currentPhase = nextPhase;
}

void GameState::changePlayerTurn(int nextPlayer)
{
    // Invalid nextPlayer
    if (nextPlayer > _playerAmount) {
        return;
    }
    _currentPlayer = nextPlayer;
}

}
