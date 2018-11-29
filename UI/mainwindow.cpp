#include "mainwindow.hh"
#include "initialize.hh"
#include "pawn.hh"
#include "shark.hh"

#include "player.hh"
#include "startdialog.hh"
#include "helpers.hh"
#include "actoritem.hh"
#include "illegalmoveexception.hh"

#include <QDesktopWidget>
#include <QGridLayout>
#include <iostream>
#include <QApplication>


const static int RESO_W = 1280;
const static int RESO_H = 720;
const static std::vector<std::string> SUPPORTED_ACTORS = {"shark"};


namespace Student {


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
   setMinimumSize(RESO_W, RESO_H);
   setWindowIcon(QIcon(":/ak47_icon.png"));
}

void MainWindow::initBoard(int playersAmount)
{
    for(int playerId = 1; playerId <= playersAmount; playerId++){
        std::shared_ptr<Player> newPlayer(new Player(playerId));
        _playerVector.push_back(newPlayer);
    }

    _scene = new QGraphicsScene(this);
    QGraphicsView* view = new QGraphicsView(this);

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
    drawGameBoard();
    drawPawns();

    _gameInfoBox = new GameInfoBox(_gameState);
    connect(_gameInfoBox, &GameInfoBox::spinButtonPressed, this, &MainWindow::spinWheel);
    _scene->addWidget(_gameInfoBox);
    _gameInfoBox->move(600, -400);

    setCentralWidget(view);
    view->setScene(_scene);

    // Set initial gameState
    _gameState->changeGamePhase(Common::GamePhase::MOVEMENT);
    _gameState->changePlayerTurn(_playerVector.at(0)->getPlayerId());
    _gameInfoBox->updateGameState();
}

void MainWindow::spinWheel()
{
    _gameRunner->spinWheel();
}

HexItem* MainWindow::getHexItem(Common::CubeCoordinate coord)
{
    return _hexItems[coord];
}

void MainWindow::movePawn(Common::CubeCoordinate origin,
                          Common::CubeCoordinate target,
                          int pawnId)
{
    if (_gameState->currentGamePhase() != Common::GamePhase::MOVEMENT) {
        return;
    }

    try {
        _gameRunner->movePawn(origin, target, pawnId);
    }
    catch (Common::IllegalMoveException) {
        return;
    }

    PawnItem* pawnItem = _pawnItems.at(pawnId);
    HexItem* newParent = _hexItems.at(target);

    pawnItem->setOffset(newParent->getPawnPosition());
    pawnItem->setParent(newParent);
    _gameState->changeGamePhase(Common::GamePhase::SINKING);
}

void MainWindow::flipHex(Common::CubeCoordinate tileCoord)
{
    _gameRunner->flipTile(tileCoord);
    if(!_gameBoard->getHex(tileCoord)->getActors().empty()){
        addActorItem(_gameBoard->returnHexes().at(tileCoord));
    }
}

void MainWindow::drawGameBoard()
{
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> hexes =
            _gameBoard->returnHexes();

    for(auto hex = hexes.begin(); hex != hexes.end(); ++hex) {
        Common::CubeCoordinate cubeCoord = hex->first;
        {
            QPointF pointCenter = Helpers::cubeToPixel(cubeCoord);
            HexItem* newHex = new HexItem(HEXSIZE,
                                          hex->second,
                                          pointCenter);
            connect(newHex, &HexItem::pawnDropped, this, &MainWindow::movePawn);
            connect(newHex, &HexItem::hexFlipped, this, &MainWindow::flipHex);
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
    ActorItem* actorItem = new ActorItem((hex->getActors().at(0)->getActorType()), hex);
    _scene->addItem(actorItem);
}

}
