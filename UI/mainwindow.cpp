#include "mainwindow.hh"
#include "initialize.hh"
#include "pawn.hh"
#include "shark.hh"
#include "constants.hh"
#include "player.hh"
#include "startdialog.hh"
#include "helpers.hh"
#include "illegalmoveexception.hh"

#include <QDesktopWidget>
#include <QGridLayout>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>


namespace Student {


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
   setMinimumSize(SizeConstants::MW_SIZE);
   _view = new ZoomGraphicsView(this);
}

void MainWindow::initBoard(int playersAmount, const bool reset)
{
    _playersAmount = playersAmount;

    _gameBoard = std::shared_ptr<Student::GameBoard>(new Student::GameBoard());
    _gameState = std::shared_ptr<GameState>(new GameState(_playersAmount));

    if (!reset) {
        initPlayers();
    }

    std::vector<std::shared_ptr<Common::IPlayer>> iPlayers;
    for (auto player : _playerMap) {
        iPlayers.push_back(
                    std::static_pointer_cast<Common::IPlayer>(player.second));
    }
    _gameRunner = Common::Initialization::getGameRunner(_gameBoard,
                                                        _gameState,
                                                        iPlayers);
    _scene = new QGraphicsScene(this);

    drawGameBoard();
    drawPawns();
    setupGameInfoBox();

    setCentralWidget(_view);
    _view->setScene(_scene);
}

void MainWindow::initPlayers()
{
    for (int playerId = 1; playerId <= _playersAmount; ++playerId){
        std::shared_ptr<Player> newPlayer(new Player(playerId));
        _playerMap[playerId] = newPlayer;
    }
}

void MainWindow::setupGameInfoBox()
{
    _gameInfoBox = new GameInfoBox(_gameState, _gameRunner, _playerMap);

    // Connect all the buttons of the GameInfoBox
    connect(_gameInfoBox, &GameInfoBox::spinButtonPressed,
            this, &MainWindow::spinWheel);
    connect(_gameInfoBox, &GameInfoBox::stayHerePressed,
            this, &MainWindow::moveToSinking);
    connect(_gameInfoBox, &GameInfoBox::continueFromSpinPressed,
            this, &MainWindow::continueFromSpinning);

    _scene->addWidget(_gameInfoBox);
    _gameInfoBox->move(OtherConstants::GIBOX_OFFSET);
    _gameInfoBox->updateGameState();

}

void MainWindow::spinWheel()
{
    if (_gameState->currentGamePhase() != Common::GamePhase::SPINNING) {
        return;
    }
    try {
        std::pair<std::string,std::string> spinResult =
                _gameRunner->spinWheel();
        const std::map<std::string, QString> actorImages =
                PathConstants::ACTOR_IMAGES;
        const std::map<std::string, QString> transportImages =
                PathConstants::TRANSPORT_IMAGES;

        bool actorExists =
                _gameBoard->checkIfActorOrTransportExists(spinResult.first);

        if(spinResult.first == "dolphin"){
            _gameInfoBox->updateActor(
                        QPixmap(transportImages.at(spinResult.first)),
                        spinResult.second,
                        actorExists);
        }
        else {
            _gameInfoBox->updateActor(
                        QPixmap(actorImages.at(spinResult.first)),
                        spinResult.second,
                        actorExists);
        }
        _movesFromSpinner = spinResult.second;

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
    checkGameStatus();
    _gameState->changeGamePhase(Common::GamePhase::MOVEMENT);
    _gameState->changePlayerTurn(getNextPlayerId());
    checkGameStatus();
    _gameInfoBox->updateGameState();
}

void MainWindow::eraseTransportItem(const int transportId)
{
    _gameBoard->removeTransport(transportId);
    delete _transportItems.at(transportId);
    _transportItems.erase(transportId);
}

void MainWindow::erasePawnItem(const int pawnId)
{
    _gameBoard->removePawn(pawnId);
    delete _pawnItems.at(pawnId);
    _pawnItems.erase(pawnId);
}

int MainWindow::getNextPlayerId()
{
   int currentId = _gameState->currentPlayer();

   // bool used to skip eliminated players.
    bool playerEliminated = true;
    while (playerEliminated)
    {
        if (static_cast<unsigned>(currentId) == _playerMap.size()) {
            currentId = 1;
            playerEliminated = _playerMap.at(currentId)->getPlayerElimination();
        }
        else {
            ++currentId;
            playerEliminated = _playerMap.at(currentId)->getPlayerElimination();
        }
    }
    return currentId;
}

void MainWindow::resetPlayerMoves(int playerId)
{
    for(auto player : _playerMap){
        if(player.second->getPlayerId() == playerId){
            player.second->setActionsLeft(3);
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

    // Check if the pawn is on a transport
    if (_gameBoard->getHex(target)->getTransports().empty())
    {
        PawnItem* pawnItem = _pawnItems.at(pawnId);
        HexItem* newParent = _hexItems.at(target);

        pawnItem->setOffset(newParent->getPawnPosition());
        pawnItem->setParent(newParent);
    }
    else
    {
        std::shared_ptr<Common::Transport> transport =
                _gameBoard->getHex(target)->getTransports().at(0);
        _transportItems.at(transport->getId())->
                switchTransportIcon(_pawnItems.at(pawnId));
        _pawnItems.at(pawnId)->hide();
    }

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
    // Wrong gamePhase or targetHex already has an actor
    if ( (_gameState->currentGamePhase() != Common::GamePhase::SPINNING)
         || (!( _gameBoard->getHex(target)->getActors().empty())))
    {
        return;
    }

    try {
        _gameRunner->moveActor(origin, target, actorId, _movesFromSpinner);
    }
    catch (Common::IllegalMoveException) {
        return;
    }
    doActorAction(target, actorId);

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
    bool spinning =
            _gameState->currentGamePhase() == Common::GamePhase::SPINNING;
    int movesLeft;
    TransportItem* transportItem = _transportItems.at(transportId);
    HexItem* newParent = _hexItems.at(target);

    try {
        if (spinning) {
            movesLeft = _gameRunner->
                    moveTransportWithSpinner(origin, target, transportId,
                                             _movesFromSpinner);
            transportItem->setPos(newParent->getActorPosition());
        }
        else {
            movesLeft = _gameRunner->moveTransport(origin, target, transportId);
            transportItem->setPos(newParent->getPawnPosition());
        }
    } catch (Common::IllegalMoveException) {
        return;
    }

    transportItem->setParent(newParent);
    _gameInfoBox->updateGameState();

    if ( (movesLeft == 0) && spinning) {
        continueFromSpinning();
    }
    else if (movesLeft == 0) {
        moveToSinking();
    }
}

void MainWindow::flipHex(const Common::CubeCoordinate &tileCoord)
{
    if (_gameState->currentGamePhase() != Common::GamePhase::SINKING) {
        return;
    }
    std::string actorType;
    try {
        actorType = _gameRunner->flipTile(tileCoord);
    }
    catch (Common::IllegalMoveException) {
        return;
    }
    _hexItems.at(tileCoord)->flip();

    const std::map<std::string, QString> actorImages =
            PathConstants::ACTOR_IMAGES;
    const std::map<std::string, QString> transportImages =
            PathConstants::TRANSPORT_IMAGES;

    if (actorType == "vortex") {
        doTheVortex(tileCoord);
    }
    else if (actorImages.find(actorType) != actorImages.end())
    {
        addActorItem(_gameBoard->returnHexes().at(tileCoord));
    }
    else if (transportImages.find(actorType) != transportImages.end())
    {
        addTransportItem(_gameBoard->returnHexes().at(tileCoord));
    }

    _gameState->changeGamePhase(Common::GamePhase::SPINNING);
    _gameInfoBox->updateGameState();
    checkGameStatus();

}

void MainWindow::drawGameBoard()
{
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> hexes =
            _gameBoard->returnHexes();

    for (auto hex = hexes.begin(); hex != hexes.end(); ++hex)
    {
        Common::CubeCoordinate cubeCoord = hex->first;
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


void MainWindow::drawPawns()
{
    Common::CubeCoordinate coord = Common::CubeCoordinate(0,0,0);

    for (const auto &player : _playerMap)
    {
        int id = player.second->getPlayerId();
        _gameBoard->addPawn(id, id, coord);
        std::shared_ptr<Common::Pawn> pawn =
                _gameBoard->getHex(coord)->givePawn(id);
        PawnItem* pawnItem = new PawnItem(
                    player.second->getPawnColor(), pawn, _hexItems[coord]);
        _pawnItems[id] = pawnItem;
        _scene->addItem(pawnItem);
    }
}

void MainWindow::addActorItem(std::shared_ptr<Common::Hex> hex)
{
    ActorItem* actorItem = new ActorItem((hex->getActors().at(0)),
                                         _hexItems.at(hex->getCoordinates()));
    _actorItems[hex->getActors().at(0)->getId()] = actorItem;
    _scene->addItem(actorItem);
}

void MainWindow::addTransportItem(std::shared_ptr<Common::Hex> hex)
{
    TransportItem* transportItem =
            new TransportItem((hex->getTransports().at(0)),
                              _hexItems.at(hex->getCoordinates()));
    _transportItems[hex->getTransports().at(0)->getId()] = transportItem;
    _scene->addItem(transportItem);
}

void MainWindow::doTheVortex(const Common::CubeCoordinate &coord)
{
    QPixmap vortexIcon(PathConstants::ACTOR_IMAGES.at("vortex"));
    QGraphicsPixmapItem* vortexItem =
            new QGraphicsPixmapItem(
                vortexIcon.scaled(SizeConstants::A_PIX_SIZE));

    QPointF coordinates = _hexItems.at(coord)->getActorPosition();

    vortexItem->setPos(coordinates);
    _scene->addItem(vortexItem);
    std::vector<Common::CubeCoordinate> coordinatesToRemoveFrom =
            _gameBoard->getHex(coord)->getNeighbourVector();
    coordinatesToRemoveFrom.push_back(coord);

    for (const auto &coordinate : coordinatesToRemoveFrom)
    {
        std::shared_ptr<Common::Hex> hex = _gameBoard->getHex(coordinate);
        for (auto transport : hex->getTransports())
        {
            int transportId = transport->getId();
            if(_transportItems.find(transportId) != _transportItems.end()){
                eraseTransportItem(transportId);
            }
        }
        for (auto pawn : hex->getPawns())
        {
            erasePawnItem(pawn->getId());
        }
        for (auto actor : hex->getActors())
        {
            int actorId = actor->getId();

            _gameBoard->removeActor(actorId);
            delete _actorItems.at(actorId);
            _actorItems.erase(actorId);
        }
    }
    QMessageBox vortex;
    vortex.setText("The vortex destroyed everything around it!");
    vortex.exec();
    delete vortexItem;
}

void MainWindow::doActorAction(const Common::CubeCoordinate &coord,
                               const int actorId)
{
    std::shared_ptr<Common::Hex> hex = _gameBoard->getHex(coord);
    std::vector<std::shared_ptr<Common::Pawn>> pawnsBefore = hex->getPawns();
    bool transport = false;
    std::shared_ptr<Common::Transport> transportBefore;

    //Only one transport per tile allowed
    if(!hex->getTransports().empty()) {
         transport = true;
         transportBefore = hex->getTransports().at(0);
    }

    _gameBoard->getActor(actorId)->doAction();

    std::vector<std::shared_ptr<Common::Pawn>> pawnsAfter = hex->getPawns();

    for(auto pawn : pawnsBefore)
    {
        if (std::find(pawnsAfter.begin(), pawnsAfter.end(), pawn) ==
                pawnsAfter.end())
        {
            erasePawnItem(pawn->getId());
        }
    }

    if (transport && hex->getTransports().empty())
    {
        int transportId = transportBefore->getId();
        TransportItem* transportItem = _transportItems.at(transportId);
        transportItem->releasePawns();
        eraseTransportItem(transportId);
    }
}

//MAYBE MOVE THIS METHOD?
void MainWindow::checkGameStatus()
{
    std::unordered_map<int, std::shared_ptr<Common::Pawn>> pawns =
            _gameBoard->getPawns();
    unsigned int pawnsLeft = pawns.size();

    if (pawnsLeft > 1)
    {
        for(auto player : _playerMap)
        {
            if (pawns.find(player.second->getPlayerId()) == pawns.end()) {
                player.second->eliminatePlayer();
            }
        }
    }
    else if (pawnsLeft == 1)
    {
        int winnerId = _gameBoard->getWinner();
        std::shared_ptr<Player> winningPlayer = _playerMap.at(winnerId);
        winningPlayer->givePoint();
        if (winningPlayer->getPoints() >= GameConstants::POINTS_FOR_WIN) {
            finishGame(winningPlayer);
        }
        else {
            newRound(winnerId);
        }
    }
    //All of the pawns have been removed, the round is a draw.
    else {
        newRound(0);
    }
}

void MainWindow::newRound(int roundWinnerId)
{
    QMessageBox newRound;

    if (roundWinnerId == 0) {
        newRound.setText("A draw! \n New round will start");
    }
    else {
        newRound.setText("Round winner was Player " +
                         QString::number(roundWinnerId) +
                         "\n New round will start");
    }
    newRound.exec();

    // Reset board only after the dropEvent has been processed completely
    QTimer::singleShot(0, this, [this] () {
        // Qt's Object Tree makes sure all items and GameInfoBox are destroyed
        // as scene is parent for them all.
        delete _scene;

        _hexItems.clear();
        _pawnItems.clear();
        initBoard(_playersAmount, true);
    });
}

void MainWindow::finishGame(std::shared_ptr<Player> winner)
{
    QMessageBox gameWon;
    gameWon.setText("Player " + QString::number(winner->getPlayerId()) +
                    " has won the game!");
    gameWon.exec();
    qApp->quit();
}

}
