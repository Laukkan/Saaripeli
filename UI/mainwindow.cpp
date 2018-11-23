#include "mainwindow.hh"
#include "igamerunner.hh"
#include "initialize.hh"
#include "pawn.hh"
#include "hexitem.hh"
#include "gamestate.hh"
#include "player.hh"
#include "startdialog.hh"
#include "helpers.hh"
#include "pawnitem.hh"
#include "actoritem.hh"

#include <QDesktopWidget>
#include <QGridLayout>


const static int RESO_W = 1280;
const static int RESO_H = 720;


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

   std::shared_ptr<Student::GameBoard> gameBoard(new Student::GameBoard());
   std::shared_ptr<GameState> gameState(new GameState);



   // Muutetaan _playerVector vektoriksi, jossa playerit ovat IPlayereitä.
   std::vector<std::shared_ptr<Common::IPlayer>> iplayers;
   for (std::shared_ptr<Player> player : _playerVector) {
       iplayers.push_back(std::static_pointer_cast<Common::IPlayer>(player));
   }

   std::shared_ptr<Common::IGameRunner> gameRunner =
           Common::Initialization::getGameRunner(gameBoard, gameState, iplayers);
   drawGameBoard(scene, gameBoard);
   drawPawns(scene, gameBoard);


   setCentralWidget(view);
   view->setScene(scene);
}

HexItem *MainWindow::getHexItem(Common::CubeCoordinate coord)
{
    return _hexItemVector[coord];
}

void MainWindow::getPlayersFromDialog(int players)
{
    for(int playerId = 1; playerId <= players; playerId++){
        std::shared_ptr<Player> newPlayer(new Player(playerId));
        _playerVector.push_back(newPlayer);
    }
}

void MainWindow::drawGameBoard(
        QGraphicsScene* scene, std::shared_ptr<Student::GameBoard> gameBoard)
{
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> hexes =
            gameBoard->returnHexes();

    for(auto hex = hexes.begin(); hex != hexes.end(); ++hex) {
        Common::CubeCoordinate cubeCoord = hex->first;
        {
            QPointF pointCenter = Helpers::cubeToPixel(cubeCoord);
            HexItem* newHex = new HexItem(HEXSIZE,
                                          hex->second,
                                          pointCenter);
            _hexItemVector[cubeCoord] = newHex;
            scene->addItem(newHex);

            scene->addItem(new ActorItem("shark",hex->second));


        }
    }
}

/* PAWNS ARE STILL DRAWN TO THE SAME POSITION IN EACH HEX
 * PAWN AND PLAYER ID'S are the same, change this? maybe not.
 */
void MainWindow::drawPawns(QGraphicsScene *scene, std::shared_ptr<Student::GameBoard> gameBoard)
{
    Common::CubeCoordinate coord = Common::CubeCoordinate(0,0,0);
    for(std::shared_ptr<Player> player : _playerVector){
        std::shared_ptr<Common::Pawn> pawn(new Common::Pawn(player->getPlayerId(), player->getPlayerId(), coord));
        PawnItem* pawnItem = new PawnItem(pawn, _hexItemVector[coord]);
        gameBoard->getHex(coord)->addPawn(pawn);
        scene->addItem(pawnItem);

    }
}

}
