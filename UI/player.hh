#ifndef PLAYER_HH
#define PLAYER_HH

#include "iplayer.hh"
#include "constants.hh"
#include "QString"


namespace Student {

class Player : public Common::IPlayer
{

public:
    /**
     * @brief Player's constructor. Initialises the attributes.
     * @param id - the id of the Player
     */
    explicit Player(int id) :
        _id(id),
        _moveActionsleft(3),
        _points(0),
        _totalTurns(0),
        _eliminated(false),
        _pawnColor(ColorConstants::PAWN_COLORS.at(_id)) {}

    /**
     * @brief Default virtual destructor
     */
    virtual ~Player() = default;

    /**
     * @brief getPlayer gets the player id.
     * @return The identifier of the player.
     * @post Exception quarantee: nothrow
     */
    virtual int getPlayerId() const;

    /**
     * @brief getActionsLeft returns the amount of actions left for the player
     * @return amount of actions
     * @post Exception quarantee: nothrow
     */
    virtual unsigned int getActionsLeft() const;

    /**
     * @brief setActionsLeft sets the player's remaining number of actions.
     * @param actionsLeft The number of actions left.
     * @post Actions left set to parameter value. Exception quarantee: nothrow
     */
    virtual void setActionsLeft(unsigned int actionsLeft);

    /**
     * @brief getPawnColor - QString representation of the color for the Pawn.
     * @return _pawnColor
     */
    QString getPawnColor();

    /**
     * @brief givePoint gives the player 1 point.
     */
    void givePoint();

    /**
     * @brief addTurn add a turn to the players total turn count.
     */
    void addTurn();

    /**
     * @brief getPoints returns the players points.
     * @return unsigned int, the players points.
     */
    unsigned int getPoints();

    /**
     * @brief getTotalTurns returns the total turns the player has had this far.
     * @return unsigned int, number of turns the player has had.
     */
    unsigned int getTotalTurns();

    /**
     * @brief setPlayerElimination - Sets the players elimination status to true
     */
    void eliminatePlayer();

    /**
     * @brief getPlayerElimination - Returns whether the player has been eliminated.
     * @return bool - True if the player has been eliminated, false if not.
     */
    bool getPlayerElimination();

private:
    /**
     * @brief _id - the Id of the Player.
     */
    int _id;

    /**
     * @brief _actionsleft the amount of Movement actions the player has left.
     */
    unsigned int _moveActionsleft;

    /**
     * @brief points How many points (round winds) the player has.
     */
    unsigned int _points;

    /**
     * @brief _totalTurns Total turns the player has taken this game. Used
     * for ranking.
     */
    unsigned int _totalTurns;

    /**
     * @brief eliminated - Whether the player has been eliminated from the round.
     */
    bool _eliminated;

    /**
     * @brief _pawnColor - the color of the Player's PawnItem
     */
    QString _pawnColor;


};

}

#endif // PLAYER_HH
