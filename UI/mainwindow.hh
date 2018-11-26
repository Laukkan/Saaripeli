#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "player.hh"
#include "igameboard.hh"
#include "gameboard.hh"
#include "hexitem.hh"
#include "pawnitem.hh"

#include <QMainWindow>
#include <QGraphicsView>
#include <memory>
#include <map>


namespace Student {


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    HexItem* getHexItem(Common::CubeCoordinate coord);

public slots:
    void getPlayersFromDialog(int players);
    void changePawnPosition(HexItem *oldParent,
                            HexItem* newParent,
                            int pawnId);

private:
    std::vector<std::shared_ptr<Player>> _playerVector;
    std::map<Common::CubeCoordinate, HexItem*> _hexItems;
    std::shared_ptr<Student::GameBoard> _gameBoard;
    std::map<int, PawnItem*> _pawnItems;

    /**
     * @brief drawGameBoard Draws the game board by adding hexItems to the scene.
     * @param scene Scene to add the hexItems to.
     */
    void drawGameBoard(QGraphicsScene* scene);
    void drawPawns(QGraphicsScene* scene);
    void addActors(QGraphicsScene* scene);

};

}

#endif // MAINWINDOW_HH
