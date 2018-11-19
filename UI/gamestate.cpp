#include "gamestate.hh"

GameState::GameState()
{

}

GameState::~GameState()
{

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
