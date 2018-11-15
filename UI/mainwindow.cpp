#include "mainwindow.hh"
#include "hexitem.hh"


#include <QDesktopWidget>
#include <QPainter>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    //setFixedSize(1280, 900);
    setMinimumSize(1280,900);
    setWindowIcon(QIcon(":/ak47_icon.png"));

    QGraphicsScene* scene = new QGraphicsScene(this);
    QGraphicsView* view_ = new QGraphicsView(this);
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(view_);
    view_ -> setScene(scene);
    scene->setSceneRect(0,0,1280,900);
    setCentralWidget(view_);
    drawGameBoard(scene);


    //Center center(200,200);
    //HexItem* testHex = new HexItem(30, center);
    //scene->addItem(testHex);
    //view_->show();

    //_renderArea = new RenderArea(this);

    //setCentralWidget(_renderArea);
}

void MainWindow::drawGameBoard(QGraphicsScene* scene)
{
    int rows = 9;
    int middleRow = rows-(rows-1)/2;
    int middleRowWidth = 9;
    for(int row=0; row < rows; row++){
        int numberOfHexes = middleRowWidth-abs((middleRow-1-row));
        int x = scene->width() / 2 -(numberOfHexes+0.5)/2*52;
        int y = scene->height() / 2 - (rows/2*46) + (row*46);

        Center center(x, y);

        drawRowOfHexes(scene,center,numberOfHexes);
    }
}

void MainWindow::drawRowOfHexes(QGraphicsScene* scene, Center rightMostCenter, int numberOfHexes)
{
    for(int hex=0; hex < numberOfHexes; hex++){
        HexItem* newHex = new HexItem(30, rightMostCenter);
        scene->addItem(newHex);
        rightMostCenter.x += 52;
    }
}
