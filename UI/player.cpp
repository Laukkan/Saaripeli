#include "player.hh"

Player::Player(int id):
    _id(id)
{

}

Player::~Player()
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
