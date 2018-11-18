#include "mainwindow.hh"
#include "gameboard.hh"
#include "igamerunner.hh"
#include "initialize.hh"
#include "gamestate.hh"
#include "player.hh"
#include <QDesktopWidget>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(1280,900);
   setWindowIcon(QIcon(":/ak47_icon.png"));

   QGraphicsScene* scene = new QGraphicsScene(this);
   QGraphicsView* view_ = new QGraphicsView(this);
   //QGridLayout* layout = new QGridLayout(this);

   std::shared_ptr<GameBoard> gameBoard(new GameBoard);
   std::shared_ptr<GameState> gameState(new GameState);
   std::vector<std::shared_ptr<Common::IPlayer>> playerVector;
   std::shared_ptr<Common::IGameRunner> gameRunner = Common::Initialization::getGameRunner(gameBoard, gameState, playerVector);
   //layout->addWidget(view_);
   //scene->setSceneRect(0,0,1280,900);
   setCentralWidget(view_);
   gameBoard->drawGameBoard(scene);
   view_ -> setScene(scene);

    /*setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setFixedSize(1280, 900);
    setWindowIcon(QIcon(":/ak47_icon.png"));

    _renderArea = new RenderArea(this);

    setCentralWidget(_renderArea);*/
}
