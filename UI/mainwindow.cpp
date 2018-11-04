#include "mainwindow.hh"

#include <QDesktopWidget>
#include <QPainter>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    resize(QDesktopWidget().availableGeometry(this).size() * 0.6);
    setWindowIcon(QIcon(":/ak47_icon.png"));

    _renderArea = new RenderArea();

    setCentralWidget(_renderArea);
}