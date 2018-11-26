#include "pawnitem.hh"
#include "helpers.hh"

#include <QDrag>
#include <QCursor>
#include <QMimeData>
#include <QPainter>

namespace Student {

PawnItem::PawnItem(std::shared_ptr<Common::Pawn> pawn, HexItem* hexItem):
    _pawn(pawn), _hexItem(hexItem)
{
    _pawnImage.load(":/pawn.png");
    setPixmap(_pawnImage.scaled(30,46));
    setOffset(_hexItem->getPawnPosition());
}

void PawnItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setCursor(Qt::ClosedHandCursor);

    event->accept();
}

void PawnItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setCursor(Qt::OpenHandCursor);
}

void PawnItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QDrag* drag = new QDrag(parentObject());
    QMimeData* mime = new QMimeData;
    drag->setMimeData(mime);


    // Greyed out picture of the pawn
    QPixmap tempPixmap = _pawnImage;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(_pawnImage.rect(), QColor(127, 127, 127, 127));
    painter.end();
    mime->setImageData(tempPixmap);

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
        setParentItem(nullptr);
    }
    setCursor(Qt::OpenHandCursor);
}

}
