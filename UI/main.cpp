#include "gameboard.hh"
#include "gamestate.hh"
#include "player.hh"
#include <mainwindow.hh>

#include <memory>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    Student::MainWindow m;
    m.show();

    return a.exec();
}
