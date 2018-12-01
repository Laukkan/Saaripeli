#include "player.hh"
#include "constants.hh"

#include <map>


namespace Student {


Player::Player(int id):
    _id(id), _actionsleft(3)
{
    _pawnColor = ColorConstants::PAWN_COLORS.at(_id);
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

QString Player::getPawnColor()
{
    return _pawnColor;
}

}
