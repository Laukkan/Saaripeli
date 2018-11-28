#include "player.hh"
#include <map>

const static std::map<int,QString> PAWN_COLORS {
    {1    , "White"},
    {2    , "Blue"},
    {3    , "Red"}
};


namespace Student {


Player::Player(int id):
    _id(id), _actionsleft(3)
{
    _pawnColor = PAWN_COLORS.at(_id);
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
