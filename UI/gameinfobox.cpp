#include "gameinfobox.hh"
#include "helpers.hh"
#include "constants.hh"
#include "actor.hh"

#include <QSizePolicy>
#include <QApplication>
#include <QThread>


namespace Student {


GameInfoBox::GameInfoBox(std::shared_ptr<GameState> gameState,
                         std::shared_ptr<Common::IGameRunner> gameRunner,
                         std::map<int, std::shared_ptr<Player>> playerMap):
    _gameState(gameState), _gameRunner(gameRunner), _playerMap(playerMap)
{
    _layout = new QGridLayout(this);
    setTitle("Game Information");

    QSizePolicy spFixed = sizePolicy();
    spFixed.setHorizontalPolicy(QSizePolicy::Policy::Fixed);
    spFixed.setVerticalPolicy(QSizePolicy::Policy::Fixed);
    setSizePolicy(spFixed);
    resize(SizeConstants::INFO_BOX_SIZE);

    initLabelsButtons();
    setupLayout();

    for (const auto &path : PathConstants::M_ACTOR_IMAGES) {
        _actorImages.push_back(QPixmap(path.second));
    }
}

void GameInfoBox::initLabelsButtons()
{
    _gamePhaseLabel = new QLabel(
                Helpers::gamePhaseToQString(_gameState->currentGamePhase()));

    _playerTurnLabel = new QLabel(
                "Player " + QString::number(_gameState->currentPlayer()));

    _playerMovesLabel = new QLabel(
                "Moves left: " + QString::number(_gameRunner->
                                getCurrentPlayer()->getActionsLeft()));

    _scoreBoardLabel = new QLabel("Score Board");
    for(auto player : _playerMap){
        QLabel* scoreLabel = new QLabel();
        _playerScoreLabels[player.second->getPlayerId()] = scoreLabel;
    }
    setPlayerPoints();

    _spinButton = new QPushButton("Spin");
    connect(_spinButton, &QPushButton::pressed,
            this, &GameInfoBox::spinButtonPressed);

    _stayHereButton = new QPushButton("Stay here");
    connect(_stayHereButton, &QPushButton::pressed,
            this, &GameInfoBox::stayHerePressed);

    _continueFromSpin = new QPushButton("Ok");
    connect(_continueFromSpin, &QPushButton::pressed,
            this, &GameInfoBox::continueFromSpinPressed);

    _actorNonExistant = new QLabel("Actor hasn't been revealed yet");
    _actorImageLabel = new QLabel();
    _actorMovesLabel = new QLabel();

    // The layout of the Groupbox is retained even when actorImage
    // is hidden
    QSizePolicy spRetain = _actorImageLabel->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    spRetain.setHorizontalPolicy(QSizePolicy::Policy::Fixed);
    _actorImageLabel->setSizePolicy(spRetain);
}

void GameInfoBox::setPlayerPoints()
{
    for(auto player : _playerMap){
        int id =player.second->getPlayerId();
        _playerScoreLabels.at(id)->clear();
        _playerScoreLabels.at(id)->setText(
                    "Player " + QString::number(player.second->getPlayerId())
                     + ": " + QString::number(player.second->getPoints()));
    }
}

void GameInfoBox::setupLayout()
{
    _layout->addWidget(_gamePhaseLabel, 0, 0);
    _layout->addWidget(_playerTurnLabel, 1, 0);
    _layout->addWidget(_playerMovesLabel, 1, 1);
    _layout->addWidget(_actorImageLabel, 2, 0, 3, 2);
    _layout->addWidget(_actorMovesLabel, 3, 2);
    _layout->addWidget(_spinButton, 5, 0);
    _layout->addWidget(_stayHereButton, 5, 1);
    _layout->addWidget(_continueFromSpin, 5, 1);
    _layout->addWidget(_actorNonExistant, 5, 0);
    _layout->addWidget(_scoreBoardLabel, 6, 0);
    int row = 7;
    for(auto label : _playerScoreLabels){
        _layout->addWidget(label.second, row, 0);
        row++;
    }

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
    //
    if (currentPhase == Common::GamePhase::SPINNING){
        _spinButton->show();
        _actorMovesLabel->show();
    }
    else {
        _actorImageLabel->clear();
        _actorMovesLabel->clear();
        _continueFromSpin->hide();
        _spinButton->hide();
        _actorNonExistant->hide();
        _actorImageLabel->hide();
        _actorMovesLabel->hide();
    }
    _gamePhaseLabel->setText(
                Helpers::gamePhaseToQString(_gameState->currentGamePhase()));

    int currentPlayerId =_gameState->currentPlayer();
    _playerTurnLabel->setText(
                "Player " + QString::number(currentPlayerId) + " ("
                + ColorConstants::PAWN_COLORS.at(currentPlayerId) + ")");

    _playerMovesLabel->setText(
                "Moves left: " + QString::number(_gameRunner->
                                getCurrentPlayer()->getActionsLeft()));

    setPlayerPoints();

}

void GameInfoBox::shuffleImages()
{
    unsigned imageAmount = static_cast<unsigned>(Helpers::randomNumber(10, 15));
    unsigned imageTime = OtherConstants::ANIM_TIME/imageAmount;

    for (unsigned i = 0; i < imageAmount; i++) {
        QPixmap rPixmap = Helpers::selectRandomImage(_actorImages.begin(),
                                                      _actorImages.end());
        _actorImageLabel->setPixmap(Helpers::scaleActorImage(rPixmap,3));
        repaint();
        QApplication::processEvents();
        QThread::msleep(imageTime);
    }
}

void GameInfoBox::updateActor(const QPixmap &image, const std::string &moves,
                              const bool actorExists)
{
    _spinButton->hide();
    _actorImageLabel->show();
    _actorNonExistant->hide();

    shuffleImages();

    // Set the correct image at the end
    _actorImageLabel->setPixmap(Helpers::scaleActorImage(image, 3));

    if (!actorExists) {
        _continueFromSpin->setText("Ok");
        _continueFromSpin->show();
        _actorNonExistant->show();
    }
    else {
        _continueFromSpin->setText("Don't move");
        _continueFromSpin->show();
        _actorMovesLabel->setText(QString::fromStdString(moves));
    }

}

}
