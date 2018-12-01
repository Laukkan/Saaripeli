#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "player.hh"
#include "igameboard.hh"
#include "igamerunner.hh"

#include "gameboard.hh"
#include "hexitem.hh"
#include "pawnitem.hh"
#include "actoritem.hh"
#include "transportitem.hh"
#include "gamestate.hh"

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
    /**
     * @brief MainWindows constructor. Sets the minimum size of the MainWindow.
     * @param parent - MainWindows parent, used in Qt's object tree.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
      * @brief Default virtual destructor
      */
    virtual ~MainWindow() = default;

    /**
     * @brief getNextPlayerId - works out the playerId who is next in turn
     * @return
     */
    int getNextPlayerId();

    /**
     * @brief resetPlayerMoves - resets the given Player's moves to 3
     * @param playerId - the id corresponding to the given Player.
     */
    void resetPlayerMoves(int playerId);

    /**
     * @brief initBoard initialises the board
     * @param playersAmount - The amount of players to initilise the board with.
     * Is given by the StartDialog.
     */
    void initBoard(int playersAmount);

public slots:
    /**
     * @brief Movement of the Hex-/Actor-/TransportItems
     * @details Called by the signal emitted by HexItem's dropEvent
     * @param origin - start coordinate
     * @param target - target coordinate
     * @param pawn-/actor-/transportId - tells what is getting moved
     */
    void movePawn(Common::CubeCoordinate origin,
                  Common::CubeCoordinate target,
                  int pawnId);
    void moveActor(Common::CubeCoordinate origin,
                  Common::CubeCoordinate target,
                  int actorId);
    void moveTransport(Common::CubeCoordinate origin,
                  Common::CubeCoordinate target,
                  int transportId);

    /**
     * @brief flipHex - Flip a given HexItem
     * @details Called by the signal emitted by HexItem's mousePressEvent
     * @param tileCoord - HexItem's coordinate
     */
    void flipHex(const Common::CubeCoordinate &tileCoord);

    /**
     * @brief spinWheel - spins the GameEngine side of the wheel and handles
     * the results.
     * @details Called by GameInfoBox's SpinButtons's pressed signal
     */
    void spinWheel();

    /**
     * @brief moveToSinking - Is called when the player doesn't want to use all
     * their moves. Starts the Sinking GamePhase.
     * @details Called by GameInfoBox's stayHereButton's pressed signal and
     * explicitly in movePawn.
     */
    void moveToSinking();

    /**
     * @brief continueFromSpinning is called when the Player finishes his last
     * phase of the game
     * @details Called by GameInfoBox's continueFromNoActor button's pressed
     * signal. Also called in explicitly in moveActor.
     */
    void continueFromSpinning();



private:
    /**
     * @brief setupGameInfoBox - sets up the GameInfoBox
     * @details used by the constructor
     */
    void setupGameInfoBox();

    /**
     * @brief drawGameBoard Draws the game board by adding hexItems to the scene.
     */
    void drawGameBoard();

    /**
     * @brief drawPawns Draws the PawnItems to the center HexItem
     */
    void drawPawns();

    /**
     * @brief Adds an *method name* to the corresponding HexItem
     * @param hex - GameEngine side implementation for HexItem
     */
    void addActorItem(std::shared_ptr<Common::Hex> hex);
    void addTransportItem(std::shared_ptr<Common::Hex> hex);

    /**
     * @brief addVortex - adds a vortex to the given coordinate
     * @param coord - CubeCoordinate represation of the coordinate
     */
    void doTheVortex(const Common::CubeCoordinate &coord);

    void doActorAction(Common::CubeCoordinate coord, int actorId);

    std::string _movesFromSpinner;

    /**
     * @brief Data structures for storing all the UI's Items
     */
    std::vector<std::shared_ptr<Player>> _playerVector;
    std::map<Common::CubeCoordinate, HexItem*> _hexItems;
    std::map<int, PawnItem*> _pawnItems;
    std::map<int, ActorItem*> _actorItems;
    std::map<int, TransportItem*> _transportItems;

    /**
     * @brief Pointers to all of the required GameEngine stuff
     */
    std::shared_ptr<Student::GameBoard> _gameBoard;
    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<Common::IGameRunner> _gameRunner;

    /**
     * @brief Mainwindow's graphical components
     */
    QGraphicsScene* _scene;
    GameInfoBox* _gameInfoBox;
};

}

#endif // MAINWINDOW_HH
