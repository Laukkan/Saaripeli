#include "mainwindow.hh"
#include "gameboard.hh"
#include "igamerunner.hh"
#include "initialize.hh"
#include "gamestate.hh"
#include "player.hh"
#include <startdialog.hh>

#include <QDesktopWidget>
#include <QGridLayout>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(1280,900);
   setWindowIcon(QIcon(":/ak47_icon.png"));
   StartDialog* startDialog = new StartDialog;
   connect(startDialog, &StartDialog::confirmed, this, &MainWindow::getPlayersFromDialog);
   startDialog->exec();

   QGraphicsScene* scene = new QGraphicsScene(this);
   QGraphicsView* view = new QGraphicsView(this);

   std::shared_ptr<Student::GameBoard> gameBoard(new Student::GameBoard(HEX_SIZE));
   std::shared_ptr<GameState> gameState(new GameState);

   // Muutetaan _playerVector vektoriksi, jossa playerit ovat IPlayereitä.
   std::vector<std::shared_ptr<Common::IPlayer>> iplayers;
   for (std::shared_ptr<Player> player : _playerVector) {
       iplayers.push_back(std::static_pointer_cast<Common::IPlayer>(player));
   }

   std::shared_ptr<Common::IGameRunner> gameRunner =
           Common::Initialization::getGameRunner(gameBoard, gameState, iplayers);

   setCentralWidget(view);
   gameBoard->drawGameBoard(scene);
   view -> setScene(scene);
}

void MainWindow::getPlayersFromDialog(int players)
{
    for(int playerId = 1; playerId <= players; playerId++){
        std::shared_ptr<Player> newPlayer(new Player(playerId));
        _playerVector.push_back(newPlayer);
    }
}
