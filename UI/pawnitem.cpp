#include "pawnitem.hh"
#include "helpers.hh"

#include <QDrag>
#include <QCursor>
#include <QMimeData>
#include <QPainter>

namespace Student {

PawnItem::PawnItem(std::shared_ptr<Common::Pawn> pawn, HexItem* parent):
    _pawn(pawn)
{
    setParent(parent);
    _pawnImage.load(":/pawn.png");
    setPixmap(_pawnImage.scaled(30,46));
    setOffset(parent->getPawnPosition());

    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setCursor(Qt::OpenHandCursor);
}

std::shared_ptr<Common::Pawn> PawnItem::returnPawn()
{
    return _pawn;
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
    mime->setText(QString::number(_pawn->getId()));

    drag->setPixmap(_pawnImage);
    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

}
