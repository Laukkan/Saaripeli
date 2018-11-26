#include "mainwindow.hh"
#include "igamerunner.hh"
#include "initialize.hh"
#include "pawn.hh"
#include "shark.hh"
#include "hexitem.hh"
#include "gamestate.hh"
#include "player.hh"
#include "startdialog.hh"
#include "helpers.hh"
#include "pawnitem.hh"
#include "actoritem.hh"

#include <QDesktopWidget>
#include <QGridLayout>
#include <iostream>


const static int RESO_W = 1280;
const static int RESO_H = 720;
const static std::vector<std::string> SUPPORTED_ACTORS = {"shark"};


namespace Student {


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
   setMinimumSize(RESO_W, RESO_H);
   setWindowIcon(QIcon(":/ak47_icon.png"));

   StartDialog startDialog;
   connect(&startDialog, &StartDialog::confirmed,
           this, &MainWindow::getPlayersFromDialog);
   startDialog.exec();

   QGraphicsScene* scene = new QGraphicsScene(this);
   QGraphicsView* view = new QGraphicsView(this);

   _gameBoard = std::shared_ptr<Student::GameBoard>(new Student::GameBoard());
   std::shared_ptr<GameState> gameState(new GameState);



   // Muutetaan _playerVector vektoriksi, jossa playerit ovat IPlayereitä.
   std::vector<std::shared_ptr<Common::IPlayer>> iplayers;
   for (std::shared_ptr<Player> player : _playerVector) {
       iplayers.push_back(std::static_pointer_cast<Common::IPlayer>(player));
   }

   std::shared_ptr<Common::IGameRunner> gameRunner =
           Common::Initialization::getGameRunner(_gameBoard, gameState, iplayers);
   drawGameBoard(scene);
   drawPawns(scene);
   addActors(scene);


   setCentralWidget(view);
   view->setScene(scene);
}

HexItem *MainWindow::getHexItem(Common::CubeCoordinate coord)
{
    return _hexItems[coord];
}

void MainWindow::getPlayersFromDialog(int players)
{
    for(int playerId = 1; playerId <= players; playerId++){
        std::shared_ptr<Player> newPlayer(new Player(playerId));
        _playerVector.push_back(newPlayer);
    }
}

PawnItem *MainWindow::getPawnItem(int pawnId)
{
    return _pawnItems[pawnId];
}

void MainWindow::drawGameBoard(QGraphicsScene* scene)
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
            _hexItems[cubeCoord] = newHex;
            scene->addItem(newHex);
        }
    }
}


void MainWindow::drawPawns(QGraphicsScene *scene)
{
    Common::CubeCoordinate coord = Common::CubeCoordinate(0,0,0);
    for(std::shared_ptr<Player> player : _playerVector){
        int id = player->getPlayerId();
        std::shared_ptr<Common::Pawn> pawn(new Common::Pawn(id, id, coord));
        PawnItem* pawnItem = new PawnItem(pawn, _hexItems[coord]);
        _pawnItems[id] = pawnItem;
        _gameBoard->getHex(coord)->addPawn(pawn);
        scene->addItem(pawnItem);
    }
}

void MainWindow::addActors(QGraphicsScene* scene)
{
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> hexes =
            _gameBoard->returnHexes();

    int actorId = 1;
    for(auto hex = hexes.begin(); hex != hexes.end(); ++hex) {
        {
            //TODO when more actors are in this should choose randomly
            //random choice could be moved to the actoritem class;
            //int randomIndex = rand() % (SUPPORTED_ACTORS.size()-1);
            ActorItem* actorItem = new ActorItem(
                        SUPPORTED_ACTORS.at(0), hex->second);
            //TODO how to determine what class of actor is made here.
            hex->second->addActor(
                        std::make_shared<Common::Shark>(Common::Shark(actorId)));
            scene->addItem(actorItem);
            actorItem->hide();
            connect(_hexItems.at(hex->first), &HexItem::turned,
                    actorItem, &ActorItem::showActor);
        }
    }
}

}
