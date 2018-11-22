#include "pawnitem.hh"


namespace Student {

PawnItem::PawnItem(std::shared_ptr<Common::Pawn> pawn, HexItem* hexItem):
    _pawn(pawn)
{
    _pawnImage.load(":/pawn.png");
    setPixmap(_pawnImage.scaled(30,46));
    setOffset(hexItem->getPawnPosition());
}

}
