#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsView>
#include <memory>
#include "player.hh"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:
    void getPlayersFromDialog(int players);

private:
    std::vector<std::shared_ptr<Player>> _playerVector;

};

#endif // MAINWINDOW_HH
