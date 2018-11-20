#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "player.hh"
#include <igameboard.hh>
#include <gameboard.hh>

#include <QMainWindow>
#include <QGraphicsView>
#include <memory>


namespace Student {


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void getPlayersFromDialog(int players);

private:
    std::vector<std::shared_ptr<Player>> _playerVector;

    /**
     * @brief drawGameBoard Draws the game board by adding hexItems to the scene.
     * @param scene Scene to add the hexItems to.
     */
    void drawGameBoard(
            QGraphicsScene* scene, std::shared_ptr<Student::GameBoard> gameBoard);

    /**
     * @brief cube_to_pixel transforms cubeCoordinates into pixel coordinates.
     * The pixel coordinates depend on the hexes size.
     * @param cubeCoord The cube cordinates to be changed to pixel coordinates.
     * @return QPointF, cube coordinates changed in to pixel coordinates.
     */
    QPointF cubeToPixel(Common::CubeCoordinate cubeCoord);

};

}

#endif // MAINWINDOW_HH
