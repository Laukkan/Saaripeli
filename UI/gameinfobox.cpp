#include "gameinfobox.hh"
#include "helpers.hh"
namespace Student {

GameInfoBox::GameInfoBox(std::shared_ptr<GameState> gameState):
    _gameState(gameState)
{
    _layout = new QGridLayout();

    _gamePhaseLabel = new QLabel(Helpers::gamePhaseToQString(_gameState->currentGamePhase()));
    _layout->addWidget(_gamePhaseLabel);

    _playerTurnLabel = new QLabel(QString::number(_gameState->currentPlayer()));
    _layout->addWidget(_playerTurnLabel);

    this->setLayout(_layout);
}

void GameInfoBox::updateGameState(){
    _gamePhaseLabel->setText(Helpers::gamePhaseToQString(_gameState->currentGamePhase()));
    _playerTurnLabel->setText("Player " + QString::number(_gameState->currentPlayer()));
}

}
