#include "gameinfobox.hh"
#include "helpers.hh"
namespace Student {

GameInfoBox::GameInfoBox(std::shared_ptr<GameState> gameState):
    _gameState(gameState)
{
    _layout = new QGridLayout();
    this->setTitle("Game Information");
    this->resize(100,50);

    _gamePhaseLabel = new QLabel(Helpers::gamePhaseToQString(_gameState->currentGamePhase()));
    _layout->addWidget(_gamePhaseLabel);

    _playerTurnLabel = new QLabel(QString::number(_gameState->currentPlayer()));
    _layout->addWidget(_playerTurnLabel);

    _spinButton = new QPushButton("Spin");
    _layout->addWidget(_spinButton);
    connect(_spinButton, &QPushButton::pressed, this, &GameInfoBox::emitSpinButtonPressed);

    this->setLayout(_layout);
}

void GameInfoBox::updateGameState(){
    _gamePhaseLabel->setText(Helpers::gamePhaseToQString(_gameState->currentGamePhase()));
    _playerTurnLabel->setText("Player " + QString::number(_gameState->currentPlayer()));
    if(_gameState->currentGamePhase() == Common::GamePhase::SPINNING){
        _spinButton->show();
    }
    else _spinButton->hide();
}

void GameInfoBox::emitSpinButtonPressed()
{
    emit spinButtonPressed();
}

}
