#include "mainwindow.hh"
#include "gameboard.hh"
#include "igamerunner.hh"
#include "initialize.hh"
#include "gamestate.hh"
#include "player.hh"
#include <QDesktopWidget>
#include <QGridLayout>

int HEX_SIZE = 30;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(1280,900);
   setWindowIcon(QIcon(":/ak47_icon.png"));

   QGraphicsScene* scene = new QGraphicsScene(this);
   QGraphicsView* view_ = new QGraphicsView(this);

   std::shared_ptr<GameBoard> gameBoard(new GameBoard(HEX_SIZE));
   std::shared_ptr<GameState> gameState(new GameState);
   std::vector<std::shared_ptr<Common::IPlayer>> playerVector;
   std::shared_ptr<Common::IGameRunner> gameRunner = Common::Initialization::getGameRunner(gameBoard, gameState, playerVector);

   setCentralWidget(view_);
   gameBoard->drawGameBoard(scene);
   view_ -> setScene(scene);
}
