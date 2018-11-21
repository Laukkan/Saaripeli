#ifndef PLAYER_HH
#define PLAYER_HH

#include "iplayer.hh"

namespace Student {


class Player : public Common::IPlayer
{
public:
    explicit Player(int id);
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

private:
    int _id;
    unsigned int _actionsleft;
};

}

#endif // PLAYER_HH
