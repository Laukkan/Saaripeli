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
#include <QGridLayout>

#include <QInputDialog>
#include <QGridLayout>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

namespace Student {


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
   setMinimumSize(SizeConstants::MW_SIZE);
   _layout = new QGridLayout();
   _centralWidget = new QWidget();
   _view = new ZoomGraphicsView();
}

void MainWindow::initBoard(int playersAmount, const bool reset)
{
    _playersAmount = playersAmount;

    _gameBoard = std::shared_ptr<Student::GameBoard>(new Student::GameBoard());
    _gameState = std::shared_ptr<GameState>(new GameState(_playersAmount));
    _spinned = false;

    if (!reset) {
        initPlayers();
    }

    std::vector<std::shared_ptr<Common::IPlayer>> iPlayers;
    for (auto player : _playerMap) {
        iPlayers.push_back(
                    std::static_pointer_cast<Common::IPlayer>(player.second));
    }
    _gameRunner = Common::Initialization::getGameRunner(_gameBoard, _gameState,
                                                        iPlayers);
    _scene = new QGraphicsScene(this);

    drawGameBoard();
    drawPawns();
    setupGameInfoBox();

    _view->setScene(_scene);
    _layout->addWidget(_view, 0, 0, 10, 10);
    _layout->addWidget(_gameInfoBox, 1, 11, 2, 2);

    _centralWidget->setLayout(_layout);
    setCentralWidget(_centralWidget);
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
    _gameInfoBox = new GameInfoBox(_gameState, _gameRunner,
                                   _playerMap, getRanking());

    // Connect all the buttons of the GameInfoBox
    connect(_gameInfoBox, &GameInfoBox::spinButtonPressed,
            this, &MainWindow::spinWheel);
    connect(_gameInfoBox, &GameInfoBox::stayHerePressed,
            this, &MainWindow::moveToSinking);
    connect(_gameInfoBox, &GameInfoBox::continueFromSpinPressed,
            this, &MainWindow::continueFromSpinning);

    _gameInfoBox->move(OtherConstants::GIBOX_OFFSET);
    _gameInfoBox->updateGameState();

}

void MainWindow::spinWheel()
{
    if (_gameState->currentGamePhase() != Common::GamePhase::SPINNING) {
        return;
    }
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
    _animalTypeFromSpinner = spinResult.first;
    _movesFromSpinner = spinResult.second;
    _spinned = true;
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
    _spinned = false;
    _gameState->changeGamePhase(Common::GamePhase::MOVEMENT);
    _playerMap.at(_gameState->currentPlayer())->addTurn();
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

void MainWindow::movePawnWithTransport(const Common::CubeCoordinate &target,
                                       const int pawnId)
{
    std::shared_ptr<Common::Transport> transport =
            _gameBoard->getHex(target)->getTransports().at(0);
    TransportItem* tItem = _transportItems.at(transport->getId());
    PawnItem* newPItem = _pawnItems.at(pawnId);


    // Switch rider
    if (transport->getTransportType() == "dolphin"
            and transport->getCapacity() == 0)
    {
        std::shared_ptr<Common::Pawn> oldPawn =
                transport->getPawnsInTransport().at(0);
        transport->removePawn(oldPawn);
    }
    // In all other events the new pawn is added
    transport->addPawn(_gameBoard->getPawn(pawnId));
    tItem->addToTransport(newPItem);
    newPItem->hide();
}

bool MainWindow::validPawnMove(const Common::CubeCoordinate &target)
{
    // Not valid if :
    //   (1) GamePhase is not right
    //   (2) or there is an actor that would eat the pawn and
    //          no transporter with room for the pawn in the hex.
    std::shared_ptr<Common::Hex> targetHex = _gameBoard->getHex(target);

    if (
            _gameState->currentGamePhase() != Common::GamePhase::MOVEMENT
         || (
                (!targetHex->getActors().empty() &&
                   (targetHex->getActors().at(0)->getActorType() != "kraken"))

         && (
                !targetHex->getTransports().empty() &&
                !targetHex->getTransports().at(0)->getCapacity())
            )
        )
    {
        return false;
    }
    return true;
}

void MainWindow::movePawn(const Common::CubeCoordinate origin,
                          const Common::CubeCoordinate target,
                          const int &pawnId)
{
    if (!validPawnMove(target)) {
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

        pawnItem->setOffset(newParent->getPawnPosition(pawnId));
        pawnItem->setParent(newParent);
    }
    else {
        movePawnWithTransport(target, pawnId);
    }

    if (movesLeft == 0) {
        moveToSinking();
        return;
    }
    _gameInfoBox->updateGameState();
}

bool MainWindow::validActorMove(const Common::CubeCoordinate &target,
                                const int actorId)
{
    // Not valid if:
    //   (1) Gamephase is wrong
    //   (2) the player hasn't spun the wheel,
    //   (3) the player is trying to move the wrong type of actor,
    //   (4) or the target hex already has an actor.

    if (
            (_gameState->currentGamePhase() != Common::GamePhase::SPINNING)
         || !_spinned
         || (_gameBoard->getActor(actorId)->getActorType()
             != _animalTypeFromSpinner
           )
         || (!( _gameBoard->getHex(target)->getActors().empty()))
    )
    {
        return false;
    }
    return true;
}

void MainWindow::moveActor(const Common::CubeCoordinate &origin,
                           const Common::CubeCoordinate &target,
                           const int actorId)
{
    if (!validActorMove(target, actorId)) {
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

bool MainWindow::validTransportMove(const bool spinning,
                                    const Common::CubeCoordinate &target,
                                    const int transportId)
{
    // Not valid if:
    //  (1) Gamephase is wrong,
    //  (2) or the player hasn't spun the wheel,
    //  (3) or the player is trying to move the wrong type of animal,
    //  (4) or the target hex has a transport
    //  (5) or the target hex has a non-shark actor (transports are
    //      immune to sharks)
    std::shared_ptr<Common::Hex> targetHex = _gameBoard->getHex(target);

    if (
            _gameState->currentGamePhase() == Common::GamePhase::SINKING
        || (spinning && !_spinned)
        || (
                spinning &&
                _gameBoard->getTransport(transportId)->getTransportType()
                != _animalTypeFromSpinner
           )
        || (!(targetHex->getTransports().empty()))
        || (
                !(targetHex->getActors().empty()) &&
                targetHex->getActors().at(0)->getActorType() != "shark"
           )
    )
    {
        return false;
    }
    return true;
}

void MainWindow::moveTransportAction(const Common::CubeCoordinate &target,
                                     const int transportId,
                                     const int movesLeft,
                                     const bool spinning)
{
    TransportItem* transportItem = _transportItems.at(transportId);
    HexItem* newParent = _hexItems.at(target);

    transportItem->setPos(newParent->getTransportPosition());
    transportItem->setParent(newParent);


    _gameInfoBox->updateGameState();

    if ( (movesLeft == 0) && spinning) {
        continueFromSpinning();
    }
    else if (movesLeft == 0) {
        moveToSinking();
    }
}

void MainWindow::moveTransport(const Common::CubeCoordinate &origin,
                               const Common::CubeCoordinate &target,
                               const int transportId)
{

    const bool spinning =
            _gameState->currentGamePhase() == Common::GamePhase::SPINNING;

    if (!validTransportMove(spinning, target, transportId)) {
        return;
    }

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
    moveTransportAction(target, transportId, movesLeft, spinning);
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

    flipHexFollowUp(tileCoord, actorType);

    _gameState->changeGamePhase(Common::GamePhase::SPINNING);
    _gameInfoBox->updateGameState();
    checkGameStatus();
}

void MainWindow::flipHexFollowUp(const Common::CubeCoordinate &tileCoord,
                                 const std::string actorType)
{
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
        doActorAction(tileCoord,_gameBoard->
                      getHex(tileCoord)->getActors().at(0)->getId());
    }
    else if (transportImages.find(actorType) != transportImages.end())
    {
        addTransportItem(_gameBoard->returnHexes().at(tileCoord));
    }
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
    vortexIcon = Helpers::scaleActorImage(vortexIcon, 3);
    QGraphicsPixmapItem* vortexItem =
            new QGraphicsPixmapItem(vortexIcon);

    QPointF coordinates = Helpers::cubeToPixel(coord);
    vortexItem->setPos(coordinates.x()-vortexIcon.width()/2,
                       coordinates.y()-vortexIcon.height()/2);

    vortexAction(coord);
    _scene->addItem(vortexItem);

    QMessageBox vortex;
    vortex.setText("The vortex destroyed everything around it!");
    vortex.exec();
    delete vortexItem;
}

void MainWindow::vortexAction(const Common::CubeCoordinate &coord)
{
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

    if (transport && hex->getTransports().empty())
    {
        int transportId = transportBefore->getId();
        TransportItem* transportItem = _transportItems.at(transportId);
        transportItem->releasePawns();
        eraseTransportItem(transportId);
    }

    for(auto pawn : pawnsBefore)
    {
        if (std::find(pawnsAfter.begin(), pawnsAfter.end(), pawn) ==
                pawnsAfter.end())
        {
            erasePawnItem(pawn->getId());
        }
    }

}

void MainWindow::checkGameStatus()
{
    unsigned int pawnsLeft = _gameBoard->pawnsLeft();

    if (pawnsLeft > 1)
    {
        for(auto player : _playerMap)
        {
            if (_gameBoard->getPawn(player.second->getPlayerId()) == nullptr) {
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
        // Qt's Object Tree makes sure all items are destroyed as scene is
        // parent to them all.
        delete _scene;
        delete _gameInfoBox;

        _hexItems.clear();
        _pawnItems.clear();

        initBoard(_playersAmount, true);
    });
}

void MainWindow::finishGame(std::shared_ptr<Player> winner)
{
    QMessageBox gameWon;
    gameWon.setText("Player " + QString::number(winner->getPlayerId()) +
                    " has won the game!\n" +
                    "They used " + QString::number(winner->getTotalTurns())
                    + " turns!");
    gameWon.exec();
    std::vector<std::vector<std::string>> ranking = getRanking();
    bool topTen = checkRanking(winner, ranking);
    if(topTen){
        updateRanking(winner,ranking);
    }
    qApp->quit();
}

void MainWindow::sortAndValidateRanking(
        std::vector<std::vector<std::string>> &ranking)
{
    // Using a lambda to sort the vector
    std::sort(ranking.begin(),ranking.end(),
              [] (const std::vector<std::string> lhs,
                  const std::vector<std::string> rhs)
    {
        return std::stoi(lhs.at(1)) < std::stoi(rhs.at(1));

    });
    //make sure its a top 10
    while (ranking.size() > 10) {
        ranking.pop_back();
    }
}

void MainWindow::openNewRanking()
{
    std::ofstream newRankingFile(PathConstants::RANKING_FILE);
    for(int lineCount = 0; lineCount < 10; lineCount++){
        newRankingFile << "-;999 \n";
    }
    newRankingFile.close();
    QMessageBox newRankingFileBox;
    newRankingFileBox.setText("A new ranking file was created");
    newRankingFileBox.exec();
}


std::vector<std::vector<std::string>> MainWindow::getRanking()
{
    std::ifstream rankingfile;
    //used to determine if a new file should be made.
    bool newfile = false;
    rankingfile.open(PathConstants::RANKING_FILE);
    std::vector<std::vector<std::string>> ranking = {};
    std::string line;
    try{
        if (rankingfile.is_open())
        {
            while (std::getline(rankingfile,line))
            {
                ranking.push_back(
                            Helpers::split(line, OtherConstants::DELIMITER));
            }
            sortAndValidateRanking(ranking);
            rankingfile.close();
        }
        //creating new file
        else {
            newfile = true;
        }
    }
    catch(...){
        QMessageBox rankingError;
        rankingError.setText("Error reading the ranking file");
        rankingError.exec();
        newfile = true;
    }
    if (newfile) {
        openNewRanking();
        ranking = getRanking();
    }
    return ranking;
}

bool MainWindow::checkRanking(std::shared_ptr<Player> winner,
                              std::vector<std::vector<std::string>> ranking)
{
    unsigned int winnerTurns = winner->getTotalTurns();
    bool topTen = false;
    for(auto player: ranking){
        if(std::stoul(player.at(1)) >= winnerTurns){
            topTen = true;
            break;
        }
    }
    return topTen;
}

void MainWindow::updateRanking(std::shared_ptr<Player> winner,
                               std::vector<std::vector<std::string>> ranking)
{
    QInputDialog top10;
    top10.setLabelText("Wow, you got to the to 10! \
                       Please enter a name to save to the rankings!");
    bool ok;
    QString playerName =
            QInputDialog::getText(
                this, "Top 10!", "Wow, player" +
                QString::number(winner->getPlayerId()) +
                " made it the to 10! \
                Please enter a name to save to the rankings!",
                QLineEdit::Normal, "", &ok, Qt::MSWindowsFixedSizeDialogHint
    );
    if (!ok or playerName.isEmpty()) {
      playerName = "Anonymous";
    }
    ranking.pop_back();
    std::vector<std::string> vectorToAdd;
    vectorToAdd.push_back(playerName.toStdString());
    vectorToAdd.push_back(std::to_string(winner->getTotalTurns()));
    ranking.push_back(vectorToAdd);
    writeRanking(ranking);

}

void MainWindow::writeRanking(std::vector<std::vector<std::string> > ranking)
{
    remove(PathConstants::RANKING_FILE.c_str());
    std::ofstream newRankingFile(PathConstants::RANKING_FILE);
    for(auto line : ranking){
        newRankingFile << line.at(0) + ";" + line.at(1) + "\n";
    }
    newRankingFile.close();
}

}
