#include "mainwindow.hh"
#include "gameboard.hh"
#include <QDesktopWidget>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(1280,900);
   setWindowIcon(QIcon(":/ak47_icon.png"));

   QGraphicsScene* scene = new QGraphicsScene(this);
   QGraphicsView* view_ = new QGraphicsView(this);
   QGridLayout* layout = new QGridLayout(this);
   GameBoard* gameboard = new GameBoard();
   layout->addWidget(view_);
   view_ -> setScene(scene);
   scene->setSceneRect(0,0,1280,900);
   setCentralWidget(view_);
   gameboard->drawGameBoard(scene);

    /*setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setFixedSize(1280, 900);
    setWindowIcon(QIcon(":/ak47_icon.png"));

    _renderArea = new RenderArea(this);

    setCentralWidget(_renderArea);*/
}
