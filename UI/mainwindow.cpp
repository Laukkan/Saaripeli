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

    // Connect all the buttons of the GameInfoBox
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
        const std::map<std::string, QString> transportImages =
                PathConstants::TRANSPORT_IMAGES;

        bool actorExists =
                _gameBoard->checkIfActorOrTransportExists(spinResult.first);

        if(spinResult.first == "dolphin"){
            _gameInfoBox->updateActor(QPixmap(transportImages.at(spinResult.first)),
                                      spinResult.second, actorExists);
        }
        else {
            _gameInfoBox->updateActor(QPixmap(actorImages.at(spinResult.first)),
                                      spinResult.second, actorExists);
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
    _gameState->changeGamePhase(Common::GamePhase::MOVEMENT);
    _gameState->changePlayerTurn(getNextPlayerId());
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
    if (_gameState->currentGamePhase() != Common::GamePhase::SPINNING
            or !(_gameBoard->getHex(target)->getActors().empty()))
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

    try {
        if (spinning) {
            movesLeft = _gameRunner->
                    moveTransportWithSpinner(origin, target, transportId,
                                             _movesFromSpinner);
        }
        else {
            movesLeft = _gameRunner->moveTransport(origin, target, transportId);
        }
    } catch (Common::IllegalMoveException) {
        return;
    }

    TransportItem* transportItem = _transportItems.at(transportId);
    HexItem* newParent = _hexItems.at(target);

    transportItem->setPos(newParent->getActorPosition());
    transportItem->setParent(newParent);
    _gameInfoBox->updateGameState();

    if (movesLeft == 0 and spinning) {
        moveToSinking();
        return;
    }
    else if (movesLeft == 0) {
        continueFromSpinning();
        return;
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

    for (std::shared_ptr<Player> player : _playerVector)
    {
        int id = player->getPlayerId();
        _gameBoard->addPawn(id, id, coord);
        std::shared_ptr<Common::Pawn> pawn =
                _gameBoard->getHex(coord)->givePawn(id);
        PawnItem* pawnItem =
                new PawnItem(player->getPawnColor(), pawn, _hexItems[coord]);
        _pawnItems[id] = pawnItem;
        _scene->addItem(pawnItem);
    }
}

void MainWindow::addActorItem(std::shared_ptr<Common::Hex> hex)
{
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
    _transportItems[hex->getTransports().at(0)->getId()] = transportItem;
    _scene->addItem(transportItem);
}

void MainWindow::doTheVortex(const Common::CubeCoordinate &coord)
{
    QPixmap vortexIcon(PathConstants::ACTOR_IMAGES.at("vortex"));
    QGraphicsPixmapItem* vortexItem =
            new QGraphicsPixmapItem(vortexIcon.scaled(SizeConstants::A_PIX_SIZE));
    QPointF coordinates = _hexItems.at(coord)->getActorPosition();

    vortexItem->setPos(coordinates);
    _scene->addItem(vortexItem);
    std::vector<Common::CubeCoordinate> coordinatesToRemoveFrom =
            _gameBoard->getHex(coord)->getNeighbourVector();
    coordinatesToRemoveFrom.push_back(coord);

    for (auto coordinate : coordinatesToRemoveFrom)
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

    if (transport and hex->getTransports().empty())
    {
        int transportId = transportBefore->getId();
        TransportItem* transportItem = _transportItems.at(transportId);
        transportItem->releasePawns();
        eraseTransportItem(transportId);
    }
}


}
