#include "mainwindow.hh"

#include <QDesktopWidget>
#include <QPainter>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setFixedSize(1280, 900);
    setWindowIcon(QIcon(":/ak47_icon.png"));

    _renderArea = new RenderArea(this);

    setCentralWidget(_renderArea);
}
