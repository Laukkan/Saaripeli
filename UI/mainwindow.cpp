#include "mainwindow.hh"
#include "igamerunner.hh"
#include "initialize.hh"
#include "pawn.hh"
#include "hexitem.hh"
#include "gamestate.hh"
#include "player.hh"
#include "startdialog.hh"

#include <QDesktopWidget>
#include <QGridLayout>

namespace Student {

/**
 * @brief _hexSize Distance from each hexes corner to their middle.
 */
const static int hexSize = 30;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(1280,900);
   setWindowIcon(QIcon(":/ak47_icon.png"));
   StartDialog startDialog;
   connect(&startDialog, &StartDialog::confirmed, this, &MainWindow::getPlayersFromDialog);
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

   setCentralWidget(view);
   view->setScene(scene);
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
            QPointF pointCenter = cubeToPixel(cubeCoord);
            HexItem* newHex = new HexItem(hexSize,
                                          hex->second,
                                          pointCenter);

            scene->addItem(newHex);
        }
    }
}

QPointF MainWindow::cubeToPixel(Common::CubeCoordinate cubeCoord)
{
    qreal q = cubeCoord.x;
    qreal r = cubeCoord.z;
    qreal x = hexSize * (sqrt(3) * q  +  sqrt(3)/2 * r);
    qreal y = hexSize * (3./2 * r);
    return QPointF(x, y);

}

}
