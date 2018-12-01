#include "gameinfobox.hh"
#include "helpers.hh"
#include "actor.hh"


namespace Student {


GameInfoBox::GameInfoBox(std::shared_ptr<GameState> gameState,
                         std::shared_ptr<Common::IGameRunner> gameRunner):
    _gameState(gameState), _gameRunner(gameRunner)
{
    _layout = new QGridLayout(this);
    setTitle("Game Information");
    resize(100,50);

    _gamePhaseLabel = new QLabel(
                Helpers::gamePhaseToQString(_gameState->currentGamePhase()));

    _playerTurnLabel = new QLabel(
                "Player " + QString::number(_gameState->currentPlayer()));

    _playerMovesLabel = new QLabel(
                "Moves left: " + QString::number(_gameRunner->
                                getCurrentPlayer()->getActionsLeft()));

    _spinButton = new QPushButton("Spin");
    connect(_spinButton, &QPushButton::pressed,
            this, &GameInfoBox::spinButtonPressed);

    _stayHereButton = new QPushButton("Stay here");
    connect(_stayHereButton, &QPushButton::pressed,
            this, &GameInfoBox::stayHerePressed);

    _actorImageLabel = new QLabel();
    _actorMovesLabel = new QLabel();

    _layout->addWidget(_gamePhaseLabel);
    _layout->addWidget(_playerTurnLabel);
    _layout->addWidget(_playerMovesLabel, 1, 1);
    _layout->addWidget(_actorImageLabel);  
    _layout->addWidget(_actorMovesLabel);
    _layout->addWidget(_spinButton);
    _layout->addWidget(_stayHereButton);

    setLayout(_layout);
}

void GameInfoBox::updateGameState(){
    Common::GamePhase currentPhase = _gameState->currentGamePhase();

    _playerMovesLabel->show();
    _stayHereButton->show();
    // Hide the move related stuff when not moving the pawns
    if (currentPhase != Common::GamePhase::MOVEMENT) {
        _playerMovesLabel->hide();
        _stayHereButton->hide();
    }
    _gamePhaseLabel->setText(
                Helpers::gamePhaseToQString(_gameState->currentGamePhase()));

    _playerTurnLabel->setText(
                "Player " + QString::number(_gameState->currentPlayer()));

    _playerMovesLabel->setText(
                "Moves left: " + QString::number(_gameRunner->
                                getCurrentPlayer()->getActionsLeft()));

    if(currentPhase == Common::GamePhase::SPINNING){
        _spinButton->show();
        _actorImageLabel->show();
        _actorMovesLabel->show();
    }
    else {
        _spinButton->hide();
        _actorImageLabel->hide();
        _actorMovesLabel->hide();
    }
}

void GameInfoBox::updateActor(QPixmap image, std::string moves)
{
    _spinButton->hide();
    _actorImageLabel->setPixmap(image.scaled(60, 92));
    _actorMovesLabel->setText(QString::fromStdString(moves));
}

}
