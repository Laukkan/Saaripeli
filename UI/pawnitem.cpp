#include "pawnitem.hh"
#include "helpers.hh"
#include "constants.hh"

#include <QDrag>
#include <QCursor>
#include <QMimeData>
#include <QPainter>
#include <QString>


namespace Student {

PawnItem::PawnItem(std::shared_ptr<Player> player,
                   std::shared_ptr<Common::Pawn> pawn,
                   HexItem* parent):
    _pawn(pawn), _player(player)
{
    _pawnImage.load(PathConstants::PAWN_IMAGES.at(_player->getPawnColor()));
    setPixmap(_pawnImage.scaled(SizeConstants::P_PIX_SIZE));
    setOffset(parent->getPawnPosition());

    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setCursor(Qt::OpenHandCursor);
    setParent(parent);
}

void PawnItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::ClosedHandCursor);
}

void PawnItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::OpenHandCursor);
}

void PawnItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    QDrag* drag = new QDrag(parent());
    QMimeData* mime = new QMimeData;
    drag->setMimeData(mime);

    // Move information of the current parent and pawn Id
    mime->setParent(parent());
    mime->setText("pawn;" + QString::number(_pawn->getId()));

    drag->setPixmap(_pawnImage);
    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

}
