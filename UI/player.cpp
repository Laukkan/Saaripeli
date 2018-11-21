#include "player.hh"

namespace Student {


Player::Player(int id):
    _id(id)
{

}

int Player::getPlayerId() const
{
    return _id;
}

unsigned int Player::getActionsLeft() const
{
    return _actionsleft;
}

void Player::setActionsLeft(unsigned int actionsLeft)
{
    _actionsleft = actionsLeft;
}

}
