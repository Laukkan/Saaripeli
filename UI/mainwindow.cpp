#include "mainwindow.hh"

#include "initialize.hh"
#include "pawn.hh"
#include "shark.hh"

#include "constants.hh"
#include "player.hh"
#include "transportitem.hh"
#include "helpers.hh"
#include "illegalmoveexception.hh"

#include <QDesktopWidget>
#include <QGridLayout>
#include <QApplication>


namespace Student {


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
   setMinimumSize(SizeConstants::MW_SIZE);
}

void MainWindow::initBoard(int playersAmount)
{
    for(int playerId = 1; playerId <= playersAmount; playerId++){
        std::shared_ptr<Player> newPlayer(new Player(playerId));
        _playerVector.push_back(newPlayer);
    }
    _gameBoard = std::shared_ptr<Student::GameBoard>(new Student::GameBoard());
    _gameState = std::shared_ptr<GameState>(new GameState);

    // Change _playerVector to a vector where the players are IPlayers.
    std::vector<std::shared_ptr<Common::IPlayer>> iplayers;
    for (std::shared_ptr<Player> player : _playerVector) {
        iplayers.push_back(std::static_pointer_cast<Common::IPlayer>(player));
    }
    _gameRunner = Common::Initialization::getGameRunner(_gameBoard,
                                                        _gameState,
                                                        iplayers);

    _scene = new QGraphicsScene(this);
    QGraphicsView* view = new QGraphicsView(this);

    drawGameBoard();
    drawPawns();

    setupGameInfoBox();

    _scene->addWidget(_gameInfoBox);
    _gameInfoBox->move(OtherConstants::GIBOX_OFFSET);

    setCentralWidget(view);
    view->setScene(_scene);

    _gameInfoBox->updateGameState();
}

void MainWindow::setupGameInfoBox()
{
    _gameInfoBox = new GameInfoBox(_gameState, _gameRunner);
    connect(_gameInfoBox, &GameInfoBox::spinButtonPressed,
            this, &MainWindow::spinWheel);
    connect(_gameInfoBox, &GameInfoBox::stayHerePressed,
            this, &MainWindow::moveToSinking);
    connect(_gameInfoBox, &GameInfoBox::continueFromSpinPressed,
            this, &MainWindow::continueFromSpinning);

}

void MainWindow::spinWheel()
{
    if (_gameState->currentGamePhase() != Common::GamePhase::SPINNING) {
        return;
    }
    try {
        std::pair<std::string,std::string> spinResult = _gameRunner->spinWheel();
        const std::map<std::string, QString> actorImages =
                PathConstants::ACTOR_IMAGES;

        //todo add dolphins in actors
        if (_gameBoard->checkIfActorExists(spinResult.first) and
                actorImages.find(spinResult.first) != actorImages.end())
        {
            _gameInfoBox->updateActor(QPixmap(actorImages.at(spinResult.first)),
                                      spinResult.second);
            _movesFromSpinner = spinResult.second;
        }
        else {
            // Update with a null pixmap to inform that the Actor doesn't exist
            _gameInfoBox->updateActor(QPixmap(), "0");
        }
    }
    catch (Common::IllegalMoveException) {
        return;
    }
}


void MainWindow::moveToSinking()
{
    resetPlayerMoves(_gameState->currentPlayer());
    _gameState->changeGamePhase(Common::GamePhase::SINKING);
    _gameInfoBox->updateGameState();
}

void MainWindow::continueFromSpinning()
{
    _gameState->changeGamePhase(Common::GamePhase::MOVEMENT);
    _gameState->changePlayerTurn(getNextPlayerId());
    _gameInfoBox->updateGameState();
}

int MainWindow::getNextPlayerId()
{
   unsigned currentId = static_cast<unsigned>(_gameState->currentPlayer());

    if(currentId == _playerVector.size()){
        return _playerVector.at(0)->getPlayerId();
    }
    else return _playerVector.at(currentId)->getPlayerId();
}

void MainWindow::resetPlayerMoves(int playerId)
{
    for(std::shared_ptr<Player> player : _playerVector){
        if(player->getPlayerId() == playerId){
            player->setActionsLeft(3);
        }
    }
}

void MainWindow::movePawn(Common::CubeCoordinate origin,
                          Common::CubeCoordinate target,
                          int pawnId)
{
    if (_gameState->currentGamePhase() != Common::GamePhase::MOVEMENT) {
        return;
    }
    int movesLeft;
    try {
        movesLeft = _gameRunner->movePawn(origin, target, pawnId);
    }
    catch (Common::IllegalMoveException) {
        return;
    }

    PawnItem* pawnItem = _pawnItems.at(pawnId);
    HexItem* newParent = _hexItems.at(target);

    pawnItem->setOffset(newParent->getPawnPosition());
    pawnItem->setParent(newParent);

    if (movesLeft == 0) {
        moveToSinking();
        return;
    }
    _gameInfoBox->updateGameState();
}

void MainWindow::moveActor(Common::CubeCoordinate origin,
                          Common::CubeCoordinate target,
                          int actorId)
{
    if (_gameState->currentGamePhase() != Common::GamePhase::SPINNING) {
        return;
    }

    try {
        _gameRunner->moveActor(origin, target, actorId, _movesFromSpinner);
    }
    catch (Common::IllegalMoveException) {
        return;
    }

    ActorItem* actorItem = _actorItems.at(actorId);
    HexItem* newParent = _hexItems.at(target);

    actorItem->setPos(newParent->getActorPosition());
    actorItem->setParent(newParent);

    continueFromSpinning();
}

void MainWindow::moveTransport(Common::CubeCoordinate origin,
                               Common::CubeCoordinate target,
                               int transportId)
{
    if (_gameState->currentGamePhase() == Common::GamePhase::SINKING) {
        return;
    }
    if (_gameState->currentGamePhase() == Common::GamePhase::SPINNING) {
        try {
            _gameRunner->moveTransportWithSpinner(origin, target,
                                                  transportId,
                                                  _movesFromSpinner);
        }
        catch (Common::IllegalMoveException) {
            return;
        }
    } else {
        try {
            _gameRunner->moveTransport(origin, target, transportId);
        }
        catch (Common::IllegalMoveException) {
            return;
        }
    }
    ActorItem* actorItem = _actorItems.at(transportId);
    HexItem* newParent = _hexItems.at(target);

    actorItem->setPos(newParent->getActorPosition());
    actorItem->setParent(newParent);
    _gameState->changeGamePhase(Common::GamePhase::SINKING);
    _gameInfoBox->updateGameState();
}

void MainWindow::flipHex(const Common::CubeCoordinate &tileCoord)
{
    if (_gameState->currentGamePhase() != Common::GamePhase::SINKING) {
        return;
    }
    try {
        std::string actorType = _gameRunner->flipTile(tileCoord);

        // The flip isn't illegal...
        _hexItems.at(tileCoord)->flip();

        const std::map<std::string, QString> actorImages =
                PathConstants::ACTOR_IMAGES;
        const std::map<std::string, QString> transportImages =
                PathConstants::TRANSPORT_IMAGES;

        if (actorImages.find(actorType) != actorImages.end() and
                actorType != "vortex")
        {
            addActorItem(_gameBoard->returnHexes().at(tileCoord));
        }
        else if (transportImages.find(actorType) != transportImages.end())
        {
            addTransportItem(_gameBoard->returnHexes().at(tileCoord));

        } else{
            // type is vortex
            addVortex(tileCoord);
        }
    }
    catch (Common::IllegalMoveException) {
        return;
    }
    _gameState->changeGamePhase(Common::GamePhase::SPINNING);
    _gameInfoBox->updateGameState();
}

void MainWindow::drawGameBoard()
{
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> hexes =
            _gameBoard->returnHexes();

    for(auto hex = hexes.begin(); hex != hexes.end(); ++hex) {
        Common::CubeCoordinate cubeCoord = hex->first;
        {
            QPointF pointCenter = Helpers::cubeToPixel(cubeCoord);
            HexItem* newHex = new HexItem(hex->second, pointCenter);
            connect(newHex, &HexItem::pawnDropped, this, &MainWindow::movePawn);
            connect(newHex, &HexItem::hexFlipped, this, &MainWindow::flipHex);
            connect(newHex, &HexItem::actorDropped, this, &MainWindow::moveActor);
            connect(newHex, &HexItem::transportDropped,
                    this, &MainWindow::moveTransport);

            _hexItems[cubeCoord] = newHex;
            _scene->addItem(newHex);
        }
    }
}


void MainWindow::drawPawns()
{
    Common::CubeCoordinate coord = Common::CubeCoordinate(0,0,0);
    for(std::shared_ptr<Player> player : _playerVector){
        int id = player->getPlayerId();
        _gameBoard->addPawn(id, id, coord);
        std::shared_ptr<Common::Pawn> pawn =
                _gameBoard->getHex(coord)->givePawn(id);
        PawnItem* pawnItem = new PawnItem(player, pawn, _hexItems[coord]);
        _pawnItems[id] = pawnItem;
        _scene->addItem(pawnItem);
    }
}

void MainWindow::addActorItem(std::shared_ptr<Common::Hex> hex)
{
    //TODO transports
    ActorItem* actorItem = new ActorItem((hex->getActors().at(0)),
                                         _hexItems.at(hex->getCoordinates()));
    _actorItems[(hex->getActors().at(0)->getId())] = actorItem;
    _scene->addItem(actorItem);
}

void MainWindow::addTransportItem(std::shared_ptr<Common::Hex> hex)
{
    //TODO transports
    TransportItem* transportItem =
            new TransportItem((hex->getTransports().at(0)),
                              _hexItems.at(hex->getCoordinates()));
    _scene->addItem(transportItem);
}

void MainWindow::addVortex(const Common::CubeCoordinate &coord)
{
    QPixmap vortexIcon(PathConstants::ACTOR_IMAGES.at("vortex"));
    QGraphicsPixmapItem* vortexItem =
            new QGraphicsPixmapItem(vortexIcon.scaled(30,46));
    QPointF coordinates = _hexItems.at(coord)->getActorPosition();
    vortexItem->setPos(coordinates);
    _scene->addItem(vortexItem);
}
}
