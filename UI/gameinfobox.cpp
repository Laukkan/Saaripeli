#include "gameinfobox.hh"
#include "helpers.hh"
#include "constants.hh"
#include "actor.hh"

#include <QSizePolicy>
#include <QApplication>
#include <QThread>


namespace Student {


GameInfoBox::GameInfoBox(std::shared_ptr<GameState> gameState,
                         std::shared_ptr<Common::IGameRunner> gameRunner):
    _gameState(gameState), _gameRunner(gameRunner)
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

    _randomGen.seed(static_cast<unsigned int>(time(nullptr)));

    for (const auto &path : PathConstants::ACTOR_IMAGES) {
        _actorImages.push_back(QPixmap(path.second));
    }
    int i = 0;
    for (const auto &path : PathConstants::TRANSPORT_IMAGES) {
        if (i >= 1) {
            // Get the non-colored images only
            break;
        }
        _actorImages.push_back(QPixmap(path.second));
        i++;
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

    _spinButton = new QPushButton("Spin");
    connect(_spinButton, &QPushButton::pressed,
            this, &GameInfoBox::spinButtonPressed);

    _stayHereButton = new QPushButton("Stay here");
    connect(_stayHereButton, &QPushButton::pressed,
            this, &GameInfoBox::stayHerePressed);

    _continueFromNoActor = new QPushButton("Ok");
    connect(_continueFromNoActor, &QPushButton::pressed,
            this, &GameInfoBox::continueFromNoActorPressed);

    _actorImageLabel = new QLabel();
    _actorMovesLabel = new QLabel();

    // The layout of the Groupbox is retained even when actorImage
    // is hidden
    QSizePolicy spRetain = _actorImageLabel->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    spRetain.setHorizontalPolicy(QSizePolicy::Policy::Fixed);
    _actorImageLabel->setSizePolicy(spRetain);
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
    _layout->addWidget(_continueFromNoActor, 5, 1);

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
        _actorImageLabel->show();
        _actorMovesLabel->show();
    }
    else {
        _actorImageLabel->clear();
        _actorMovesLabel->clear();
        _spinButton->hide();
        _continueFromNoActor->hide();
        _actorImageLabel->hide();
        _actorMovesLabel->hide();
    }
    _gamePhaseLabel->setText(
                Helpers::gamePhaseToQString(_gameState->currentGamePhase()));

    _playerTurnLabel->setText(
                "Player " + QString::number(_gameState->currentPlayer()));

    _playerMovesLabel->setText(
                "Moves left: " + QString::number(_gameRunner->
                                getCurrentPlayer()->getActionsLeft()));

}

void GameInfoBox::updateActor(QPixmap image, std::string moves)
{
    _spinButton->hide();
    _actorImageLabel->show();
    std::uniform_int_distribution<unsigned> distribut(8, 16);
    unsigned imageAmount = distribut(_randomGen);
    unsigned imageTime = OtherConstants::ANIM_TIME/imageAmount;

    for (int i = 0; i < 15; i++) {
        QPixmap rPixmap = Helpers::selectRandomImage(_actorImages.begin(),
                                                     _actorImages.end(),
                                                     _randomGen);
        _actorImageLabel->setPixmap(Helpers::scaleActorImage(rPixmap,3));
        repaint();
        QApplication::processEvents();
        QThread::msleep(imageTime);
    }


    if (image.isNull()) {
        _actorImageLabel->setText("Actor hasn't been revealed yet");
        _continueFromNoActor->setText("Ok");
        _continueFromNoActor->show();
    }
    else {
        _continueFromNoActor->setText("Stay here");
        _continueFromNoActor->show();
        _actorImageLabel->setPixmap(Helpers::scaleActorImage(image, 3));
        _actorMovesLabel->setText(QString::fromStdString(moves));
    }

}

}
