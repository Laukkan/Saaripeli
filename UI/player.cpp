#include "player.hh"

#include <map>


namespace Student {


int Player::getPlayerId() const
{
    return _id;
}

unsigned int Player::getActionsLeft() const
{
    return _moveActionsleft;
}

void Player::setActionsLeft(unsigned int actionsLeft)
{
    _moveActionsleft = actionsLeft;
}

QString Player::getPawnColor()
{
    return _pawnColor;
}

void Player::givePoint()
{
    _points += 1;
}

unsigned int Player::getPoints()
{
    return _points;
}

void Player::setPlayerElimination(bool state)
{
    _eliminated = state;
}

bool Player::getPlayerElimination()
{
    return _eliminated;
}

}
