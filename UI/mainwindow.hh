#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "player.hh"
#include "igameboard.hh"
#include "gameboard.hh"
#include "hexitem.hh"
#include "pawnitem.hh"
#include "actoritem.hh"
#include "transportitem.hh"
#include "gamestate.hh"
#include "igamerunner.hh"
#include "gameinfobox.hh"

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

    int getNextPlayerId();

    void resetPlayerMoves(int playerId);

public slots:
    void movePawn(Common::CubeCoordinate origin,
                  Common::CubeCoordinate target,
                  int pawnId);
    void moveActor(Common::CubeCoordinate origin,
                  Common::CubeCoordinate target,
                  int actorId);
    void moveTransport(Common::CubeCoordinate origin,
                  Common::CubeCoordinate target,
                  int transportId);
    void flipHex(Common::CubeCoordinate tileCoord);
    void initBoard(int playersAmount);
    void spinWheel();

    void moveToSinking();
    void continueFromSpinning();

private:
    std::vector<std::shared_ptr<Player>> _playerVector;
    std::map<Common::CubeCoordinate, HexItem*> _hexItems;
    std::map<int, PawnItem*> _pawnItems;
    std::map<int, ActorItem*> _actorItems;
    std::map<int, TransportItem*> _transportItems;

    QGraphicsScene* _scene;
    std::shared_ptr<Student::GameBoard> _gameBoard;
    GameInfoBox* _gameInfoBox;
    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<Common::IGameRunner> _gameRunner;
    std::string _movesFromSpinner;

    /**
     * @brief drawGameBoard Draws the game board by adding hexItems to the scene.
     * @param scene Scene to add the hexItems to.
     */
    void drawGameBoard();
    void drawPawns();
    void addActorItem(std::shared_ptr<Common::Hex> hex);
    void addTransportItem(std::shared_ptr<Common::Hex> hex);
    void addVortex(Common::CubeCoordinate coord);
    void doActorAction(Common::CubeCoordinate coord, int actorId);
    //void addPawnToTransport(Common::CubeCoordinate coord, int pawnId);

};

}

#endif // MAINWINDOW_HH
