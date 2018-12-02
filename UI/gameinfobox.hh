#ifndef GAMEINFOBOX_HH
#define GAMEINFOBOX_HH

#include "gamestate.hh"
#include "igamerunner.hh"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

namespace Student {

class GameInfoBox : public QGroupBox
{
    Q_OBJECT

public:
    /**
     * @brief GameInfoBox's constructor
     * @param gameState - a pointer to the current GameState of the game
     * @param gameRunner - a pointer to the GameRunner
     */
    explicit GameInfoBox(std::shared_ptr<GameState> gameState,
                         std::shared_ptr<Common::IGameRunner> gameRunner);

    /**
      * @brief Default virtual destructor. Qt's parent-mechanism takes care of
      * the destruction of all the declared labels, buttons etc.
      */
    virtual ~GameInfoBox() = default;

    /**
     * @brief updateGameState updates the GameInfoBox to the current GameState
     */
    void updateGameState();

    /**
     * @brief updateActor - updates the GameInfoBox to show the spin result
     * @param image - The Image of the actor
     * @param moves - The amount of moves the actor has.
     * @if pixmap is null the spin result doesn't exist in the GameBoard.
     * _actorImageLabel is updated to inform this result and _continueFromSpin
     * button is shown.
     */
    void updateActor(QPixmap pixmap, std::string moves);

    void informOfVortex();

signals:
    /**
    * @brief Emitted when the corresponding button is pressed
    */
   void spinButtonPressed();
   void stayHerePressed();
   void continueFromNoActorPressed();

private:
   /**
    * @brief setupLayout - sets the Layout of the GameInfoBox
    */
   void setupLayout();

   /**
    * @brief initLabelsButtons - initialises the GameInfoBox's labels and
    * buttons.
    * @details used by the constructor
    */
   void initLabelsButtons();

   /**
    * @brief Pointers to the required GameEngine implementations used in
    * this class.
    */
   std::shared_ptr<GameState> _gameState;
   std::shared_ptr<Common::IGameRunner> _gameRunner;

   /**
     * @brief Labels that are always visible.
     */
    QLabel* _gamePhaseLabel;
    QLabel* _playerTurnLabel;
    QLabel* _playerMovesLabel;

    /**
     * @brief Show the result of the spin.
     */
    QLabel* _actorImageLabel;
    QLabel* _actorMovesLabel;

    /**
     * @brief _spinButton is shown in the 3rd Spinning phase of the game
     * @details Connected to the spinDisc slot of The MainWindow
     */
    QPushButton* _spinButton;
    /**
     * @brief _stayHereButton is shown in the 1st phase of the game. The player
     * presses this button if he doesn't want to use all of his moves.
     * @details Connected to the MainWindow's moveToSinking slot
     */
    QPushButton* _stayHereButton;

    /**
     * @brief _continueFromNoActor is shown when the Player is given the message
     * that the spin result (Actor) doesn't exist on the GameBoard.
     * @details Connected to the MainWindow's continueFromSpinning slot.
     */
    QPushButton* _continueFromNoActor;

    /**
     * @brief _layout - the GroupBox's layout
     */
    QGridLayout* _layout;
};

}
#endif // GAMEINFOBOX_HH
