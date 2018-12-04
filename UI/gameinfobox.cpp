#include "gameinfobox.hh"
#include "helpers.hh"
#include "constants.hh"
#include "actor.hh"

#include <QSizePolicy>
#include <QApplication>
#include <QThread>
#include <algorithm>


namespace Student {


GameInfoBox::GameInfoBox(std::shared_ptr<GameState> gameState,
                         std::shared_ptr<Common::IGameRunner> gameRunner,
                         std::map<int, std::shared_ptr<Player>> playerMap,
                         std::vector<std::vector<std::string>> ranking):
    _gameState(gameState), _gameRunner(gameRunner), _playerMap(playerMap), _ranking(ranking)

{
    _layout = new QGridLayout(this);
    setTitle("Game Information");

    /*QSizePolicy spFixed = sizePolicy();
    spFixed.setHorizontalPolicy(QSizePolicy::Policy::Fixed);
    spFixed.setVerticalPolicy(QSizePolicy::Policy::Fixed);
    setSizePolicy(spFixed);
    resize(SizeConstants::INFO_BOX_SIZE);*/

    std::random_device rd;
    _randomGen = std::mt19937(rd());

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

    _scoreBoardLabel = new QLabel("Scoreboard");
    for(auto player : _playerMap){
        QLabel* scoreLabel = new QLabel();
        _playerScoreLabels[player.second->getPlayerId()] = scoreLabel;
    }

    _rankingLabel = new QLabel("Rankings");
    for(auto player : _ranking){
        QLabel* playerRankLabel = new QLabel(
                    QString::fromStdString(player.at(0) + " : "
                                           + player.at(1)));
        _playerRankingLabels.push_back(playerRankLabel);
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

    // Set it so that the layout of the Groupbox is retained even when
    // certain labels are hidden
    retainSizeHidden(_actorImageLabel);
    retainSizeHidden(_playerMovesLabel);
    retainSizeHidden(_actorNonExistant);
}

void GameInfoBox::retainSizeHidden(QLabel *label)
{
    QSizePolicy spRetain = label->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    label->setSizePolicy(spRetain);
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
        ++row;
    }
    _layout->addWidget(_rankingLabel,row,0);
    row++;
    for(auto label : _playerRankingLabels){
        _layout->addWidget(label, row, 0);
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
    std::vector<QPixmap>::iterator iter = _actorImages.begin();
    std::shuffle(iter, _actorImages.end(), _randomGen);

    for (unsigned i = 0; i < imageAmount; ++i, ++iter)
    {
        if (iter == _actorImages.end()) {
            iter = _actorImages.begin();
        }
        _actorImageLabel->setPixmap(Helpers::scaleActorImage(*iter, 2));
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
    _actorImageLabel->setPixmap(Helpers::scaleActorImage(image, 2));

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
